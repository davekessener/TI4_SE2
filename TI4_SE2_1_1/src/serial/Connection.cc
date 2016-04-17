#include "serial/Connection.h"
#include "serial/Packets.h"

#define MXT_LOOPDELAY ms(2)

namespace hw
{

namespace
{
	typedef Packet_ptr (*packet_fn)(const uint8_t *, uint32_t);
	
	packet_fn packet_handler[] =
	{
		&DataPacket::assemble,
		&OKPacket::assemble,
		&ErrorPacket::assemble,
		&AcknowledgePacket::assemble
	};

	void enforceWrite(int f, const void *d, size_t s)
	{
		size_t w = 0;

		while(w < s)
		{
			int t = write(f, d + w, s - w);
			if(t < 0) throw std::runtime_exception("err writing serial connection");
			w += t;
		}
	}

	int enforceRead(int f, void *d, size_t s)
	{
		int t = readcond(f, d, s, s, 0, 0);

		if(t < 0)
			throw std::runtime_exception("err reading serial connection");

		return t;
	}

	template<typename T>
	void write(int f, const T& e)
	{
		enforceWrite(f, &e, sizeof(T));
	}

	template<typename T>
	T read(int f)
	{
		T e;
		while(!enforceRead(f, &e, sizeof(T)));
		return e;
	}

	void writePacket(int f, Packet_ptr p)
	{
		write(f, p->id());
		write(f, p->size());
		enforceWrite(f, p->data(), p->size());
	}

	Packet_ptr readPacket(int f)
	{
		uint32_t id;

		if(!enforceRead(f, &id, sizeof(id)))
			return Packet_ptr;

		uint32_t size = read<uint32_t>(f);
		uint8_t *buf = new uint8_t[size];
		
		while(!enforceRead(f, buf, size));

		Packet_ptr p = packet_handler[id](buf, size);

		delete[] buf;

		return p;
	}

	void checkedWritePacket(int f_, Packet_ptr p)
	{
		Packet_ptr t;

		writePacket(f_, p);
		
		while(!static_cast<bool>(t = readPacket(f_)));

		if(t->id() != Packet::OK_ID)
			throw std::runtime_error("no confirmation on serial connection");
		else if(static_cast<OKPacket *>(t)->status() != p->hash())
			throw std::runtime_error("data transmission error");
	}

	Packet_ptr checkedReadPacket(int f)
	{
		Packet_ptr p = readPacket(f);

		if(static_cast<bool>(p))
		{
			writePacket(f, Packet_ptr(new OKPacket(p->hash())));
		}

		return p;
	}

	template<typename T>
	class FtorWrapper
	{
		public:
			FtorWrapper(T *t, void (T::*f)(void))
				: t_(t), f_(f) { }
			void operator()( ) { (t_->*f_)(); }
		private:
			T *t_;
			void (T::*f_)(void);
	};
}

// # ===========================================================================

Connection::Connection(const std::string& d, bool a) : device_(d), active_(a)
{
	if((f_ = open(device_.c_str(), O_RDWR)) < 0)
		throw std::runtime_exception("err establishing serial connection");
	
	struct termios ts;
	tcflush(f_, TCIOFLUSH);
	tcgetattr(f_, &ts);
	cfsetispeed(&ts, B19200);
	cfsetospeed(&ts, B19200);
	ts.c_cflag &= ~CSIZE;
	ts.c_cflag &= ~CSTOPB;
	ts.c_cflag &= ~PARENB;
	ts.c_cflag |= CSB;
	ts.c_cflag |= CREAD;
	ts.c_cflag |= CLOCAL;
	tcsetattr(f_, TCSANOW, &ts);

	running_ = true;
	
	thread_.set(new lib::Thread(FtorWrapper(this, &Connection::thread)));
}

Connection::~Connection(void)
{
	close();
	close(f_);
}

void Connection::write(const void *d, size_t s)
{
	wbuf_.enqueue(Packet_ptr(new DataPacket(d, s)));
}

Packet_ptr Connection::read(void)
{
	return rbuf_.dequeue();
}

void Connection::close(void)
{
	if(running_)
	{
		running_ = false;
		thread_->join();
	}
}

void Connection::thread(void)
{
	lib::Time delay = lib::Time::MXT_LOOPDELAY;

	while(running_)
	{
		if(active_)
		{
			while(!wbuf_.empty())
			{
				checkedWritePacket(f_, wbuf_.dequeue());
			}

			checkedWritePacket(f_, Packet_ptr(new AcknowledgePacket));

			active_ = false;
		}
		else
		{
			Packet_ptr p = checkedReadPacket(f_);

			if(static_cast<bool>(p))
			{
				if(p->id() == Packet::ACKNOWLEDGE_ID)
				{
					active_ = true;
				}
				else if(p->id() == Packet::DATA_ID)
				{
					rbuf_.enqueue(p);
				}
				else
				{
					writePacket(f_, Packet_ptr(new ErrorPacket));
				}
			}
			else
			{
				delay.wait();
			}
		}

		if(running_ && active_) delay.wait();
	}
}

}

