#include <memory>

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#include "serial/Connection.h"
#include "lib/TimeP.h"
#include "lib/Timer.h"
#include "lib/concurrent/Thread.h"
#include "lib/RingBuffer.hpp"
#include "lib/mpl/FtorWrapper.hpp"
#include "lib/log/LogManager.h"

#define MXT_PACKETBUFSIZE 256
#define MXT_IOSPEED B19200
#define MXT_TIMEOUT ms(100)

#define TOK_A   0x8de29457
#define TOK_OK  0x226442b8
#define TOK_HSA 0x88e7d7c9
#define TOK_HSB 0x2624d967
#define TOK_DAT 0x36773fe0

namespace hw {

using lib::Time;
using lib::Timer;
using lib::Thread;
using lib::Data_ptr;
using lib::Data;

using lib::log::LogManager;
using lib::log::LogLevel;
using lib::log::Logger_ptr;

class Connection::Impl
{
	friend class Connection;

	typedef lib::RingBuffer<Data_ptr, MXT_PACKETBUFSIZE, lib::RingBufferConcurrency::MultiThreaded> buf_t;
	
	struct DoneRunning { };

	struct Packet
	{
		explicit Packet(uint32_t t) : tag(t) { }
		Packet(uint32_t t, Data_ptr p) : tag(t), data(p) { }

		uint32_t tag;
		Data_ptr data;
	};

	void run( );
		void send(const void *pp, size_t n);
		bool try_recv(void *pp, size_t n);
		void recv(void *pp, size_t n);
		template<typename T>
			void send(const T& t);
		template<typename T>
			T recv( );
		void checkRunning( );
		void sendPacket(const Packet& p);
		Packet receivePacket( );
		void checkedSend(const Packet& p);
		Packet checkedRecv( );
		void flush( );
		void giveHandshake( );
		void receiveHandshake( );
		void sendData( );
		void recvData( );
		void resetTimer( );
		void onTimeout( );

	bool active_, running_, connected_, connectionLost_;
	int f_;
	std::auto_ptr<Thread> thread_;
	buf_t rBuf_, wBuf_;
	Logger_ptr log_;
	Timer timeout_;
};

// # ===========================================================================

namespace
{
	Logger_ptr getLog()
	{
		static Logger_ptr l = LogManager::instance().rootLog();
		return l;
	}

#define MXT_USEQNX

	int nb_read(int f, void *pp, size_t n)
	{
#ifndef MXT_USEQNX
		fd_set rfds;
		struct timeval tv;

		FD_ZERO(&rfds);
		FD_SET(f, &rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		int r = select(f + 1, &rfds, NULL, NULL, &tv);

		if(r < 0)
			throw std::string("failed to check for readability");

		if(r > 0)
		{
			r = read(f, pp, n);

			if(r == -1)
			{
				if(errno != EAGAIN && errno != EWOULDBLOCK)
					throw std::string("failed read");
				else
					r = 0;
			}
		}

		return r;
#else
		int r = readcond(f, pp, n, 0, 0, 0);

		if(r < 0)
			throw std::string("failed read");

		return r;
#endif
	}

	int nb_write(int f, const void *pp, size_t n)
	{
		int r = write(f, pp, n);

		if(r == -1)
		{
			if(errno != EAGAIN && errno != EWOULDBLOCK)
				throw std::string("failed write");
			else
				r = 0;
		}

		return r;
	}
}

// # ===========================================================================

void Connection::Impl::send(const void *pp, size_t n)
{
	size_t t = 0;
	int r = 0;
	const uint8_t *p = (const uint8_t *) pp;

	while(t < n)
	{
		r = nb_write(f_, p + t, n - t);

		checkRunning();

		t += r;
	}
}

// # ---------------------------------------------------------------------------

bool Connection::Impl::try_recv(void *pp, size_t n)
{
	size_t t = 0;
	int r = 0;
	uint8_t *p = (uint8_t *) pp;

	while(t < n)
	{
		r = nb_read(f_, p + t, n - t);

		checkRunning();

		if(!r && !t) return false;

		t += r;
	}

	return true;
}

// # ---------------------------------------------------------------------------

void Connection::Impl::recv(void *pp, size_t n)
{
	while(!try_recv(pp, n)) checkRunning();
}

// # ---------------------------------------------------------------------------

template<typename T>
void Connection::Impl::send(const T& t)
{
	send(&t, sizeof(t));
}

template<typename T>
T Connection::Impl::recv(void)
{
	T t;
	recv(&t, sizeof(t));
	return t;
}

// # ---------------------------------------------------------------------------

void Connection::Impl::checkRunning(void)
{
	if(!running_)
		throw DoneRunning();
	if(connectionLost_)
		throw std::string("timout occured!");
}

// # ---------------------------------------------------------------------------

void Connection::Impl::sendPacket(const Packet& p)
{
	send<uint32_t>(p.tag);
	if(static_cast<bool>(p.data) && p.data->size() > 0)
	{
		send<uint32_t>(p.data->size());
		send(p.data->data(), p.data->size());
	}
	else
	{
		send<uint32_t>(0);
	}
}

Connection::Impl::Packet Connection::Impl::receivePacket(void)
{
	Packet p(recv<uint32_t>());
	uint32_t size = recv<uint32_t>();

	if(size > 0)
	{
		p.data = Data::empty(size);
		recv(p.data->data(), size);
	}

	return p;
}

void Connection::Impl::checkedSend(const Packet& p)
{
	resetTimer();
	sendPacket(p);

	resetTimer();
	Packet a = receivePacket();

	if(a.tag != TOK_OK) throw std::string("invalid answer!");
}

Connection::Impl::Packet Connection::Impl::checkedRecv(void)
{
	resetTimer();
	Packet p = receivePacket();

	resetTimer();
	sendPacket(Packet(TOK_OK));

	return p;
}

void Connection::Impl::resetTimer(void)
{
	timeout_.reset();
}

void Connection::Impl::onTimeout(void)
{
	connectionLost_ = true;
}

// # ---------------------------------------------------------------------------

void Connection::Impl::flush(void)
{
	if(tcflush(f_, TCIOFLUSH) == -1)
		throw std::string("failed flush");
}

// # ---------------------------------------------------------------------------

void Connection::Impl::giveHandshake(void)
{
	uint32_t r = 0;

	while(true)
	{
		send<uint32_t>(TOK_HSA);

		Time::ms(20).wait();
		
		if(try_recv(&r, sizeof(r)))
		{
			if(r == TOK_HSB)
				break;
			else
				throw std::string("failed hand shake");
		}
	}
}

void Connection::Impl::receiveHandshake(void)
{
	uint32_t r = recv<uint32_t>();

	if(r != TOK_HSA)
		throw std::string("failed hand shake");

	send<uint32_t>(TOK_HSB);

	flush();
}

// # ---------------------------------------------------------------------------

void Connection::Impl::sendData(void)
{
	while(!wBuf_.empty())
	{
		checkedSend(Packet(TOK_DAT, wBuf_.dequeue()));
	}

	checkedSend(Packet(TOK_A));
}

void Connection::Impl::recvData(void)
{
	while(true)
	{
		Packet a = checkedRecv();

		if(a.tag == TOK_A)
			break;

		rBuf_.enqueue(a.data);
	}
}

// # ---------------------------------------------------------------------------

void Connection::Impl::run(void)
{
	running_ = true;
	connectionLost_ = false;

	try
	{
		if(active_)
			giveHandshake();
		else
			receiveHandshake();

		connected_ = true;

		resetTimer();
		timeout_.executeWhen(Time::MXT_TIMEOUT, lib::wrapInFtor(this, &Connection::Impl::onTimeout));

		while(true)
		{
			if(active_)
			{
				sendData();
				active_ = false;
			}
			else
			{
				recvData();
				active_ = true;
			}

			checkRunning();

			Time::ms(10).wait();
		}
	}
	catch(const DoneRunning& e)
	{
		getLog()->MXT_LOG("shutting down connection");
	}
	catch(const std::string& e)
	{
		getLog()->MXT_LOGL(LogLevel::ERROR, "caught exception: \"%s\" [errno %i (%s)]", e.c_str(), errno, strerror(errno));
		throw;
	}

	timeout_.deactivate();
	connected_ = false;
}

// # ---------------------------------------------------------------------------

Connection::Connection(const std::string& d, bool a)
: impl_(new Impl)
{
	impl_->active_ = a;
	impl_->running_ = false;
	impl_->connected_ = false;

	if((impl_->f_ = open(d.c_str(), O_RDWR | O_NOCTTY)) < 0)
		throw std::string("couldn't open device '" + d + "'!");

	if(fcntl(impl_->f_, F_SETFL, 0) < 0) throw std::string("fcntl");

	struct termios ts;
	if(tcflush(impl_->f_, TCIOFLUSH) < 0) throw std::string("tcflush");
	if(tcgetattr(impl_->f_, &ts) < 0) throw std::string("tcgetattr");
	if(cfsetispeed(&ts, MXT_IOSPEED) < 0) throw std::string("cfsetispeed");
	if(cfsetospeed(&ts, MXT_IOSPEED) < 0) throw std::string("cfsetospeed");
	ts.c_cflag &= ~CSIZE;
	ts.c_cflag &= ~CSTOPB;
	ts.c_cflag &= ~PARENB;
	ts.c_cflag |= CS8;
	ts.c_cflag |= CREAD;
	ts.c_cflag |= CLOCAL;

	ts.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	ts.c_iflag &= ~(IXON | IXOFF | IXANY);
//	ts.c_cflag &= ~CRTSCTS;
	ts.c_oflag &= ~OPOST;

	ts.c_oflag &= ~(ONLCR | OCRNL);
	ts.c_iflag &= ~(INLCR | ICRNL);

	if(tcsetattr(impl_->f_, TCSANOW, &ts) < 0) throw std::string("tcsetattr");

	impl_->log_ = LogManager::instance().getLog("serial");
//	impl_->log_->addParent(getLog());

	impl_->thread_.reset(new Thread(lib::wrapInFtor(impl_, &Connection::Impl::run)));
}

// # ---------------------------------------------------------------------------

Connection::~Connection( )
{
	close();

	delete impl_;
}

void Connection::close(void)
{
	Lock guard(this);

	if(impl_->f_)
	{
		impl_->running_ = false;
		impl_->thread_->join();
		impl_->thread_.reset();
		::close(impl_->f_);
		impl_->f_ = 0;
	}
}

bool Connection::connected(void) const { return impl_->connected_; }
bool Connection::running(void) const { return impl_->running_; }
bool Connection::doneWriting(void) const { return impl_->wBuf_.empty(); }

void Connection::sendData(Data_ptr p)
{
	impl_->wBuf_.enqueue(p);
}

Data_ptr Connection::receiveData(void)
{
	return impl_->rBuf_.dequeue();
}

bool Connection::hasData(void) const
{
	return !impl_->rBuf_.empty();
}

}

