#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <stdexcept>
#include <iostream>

#include "test/SerialTest.h"
#include "serial/Connection.h"
#include "lib/TimeP.h"
#include "lib/log/LogManager.h"

namespace test
{
	using hw::Connection;
	using hw::Packet_ptr;

	void serialA(void)
	{
		Connection c("/dev/ser1", true);
		std::string s("Hello, World!");

		c.write(s.c_str(), s.length());

		c.close();
	}

	void serialB(void)
	{
		Connection c("/dev/ser2", false);

		Packet_ptr p = c.read();

		char *buf = new char[p->size() + 1];
		for(int i = 0 ; i < p->size() ; ++i)
		{
			buf[i] = ((const char *) p->data())[i];
		}
		buf[p->size()] = '\0';

		std::cout << buf << std::endl;

		c.close();
	}

	int writeW(int f, const void *d, size_t s)
	{
		int r = write(f, d, s);
		if(r < 0)
			MXT_TODO_ERROR;
		return r;
	}

	int readcondW(int f, void *d, size_t s, size_t m, int t, int tt)
	{
		int r = readcond(f, d, s, m, t, tt);
		if(r < 0)
			MXT_TODO_ERROR;
		return r;
	}

	int writeS(int f, const std::string& s)
	{
		unsigned char l = s.length();
		int r = 0;
		unsigned t;

		t = 0;
		while((t += writeW(f, &l, sizeof(l))) < sizeof(l));

		r += t;
		t = 0;
		while((t += writeW(f, s.c_str() + t, l - t)) < l);

		r += t;

		return r;
	}

	std::string readS(int f, int *rr)
	{
		unsigned char l;
		int r = 0;
		unsigned t;
		char *buf = NULL;

		t = 0;
		while((t += readcondW(f, &l, sizeof(l), 0, 0, 0)) < sizeof(l));

		buf = new char[l + 1];

		r += t;
		t = 0;
		while((t += readcondW(f, buf + t, l - t, 0, 0, 0)) < l);

		r += t;
		*rr = r;
		buf[l] = '\0';

		std::string s(buf);
		delete[] buf;
		return s;
	}

	void basicSerial(bool active)
	{
		lib::log::Logger_ptr log = lib::log::LogManager::instance().rootLog();

		log->MXT_LOG("enter serial test %s", (active ? "ACTIVE" : "PASSIVE"));

		int f;
		if((f = open(active ? "/dev/ser1" : "/dev/ser2", O_RDWR)) < 0)
			throw std::runtime_error("err establishing serial connection");

		struct termios ts;
		tcflush(f, TCIOFLUSH);
		tcgetattr(f, &ts);
		cfsetispeed(&ts, B19200);
		cfsetospeed(&ts, B19200);
		ts.c_cflag &= ~CSIZE;
		ts.c_cflag &= ~CSTOPB;
		ts.c_cflag &= ~PARENB;
		ts.c_cflag |= CS8;
		ts.c_cflag |= CREAD;
		ts.c_cflag |= CLOCAL;
		tcsetattr(f, TCSANOW, &ts);

		int w = 0;

		if(active)
		{
			w = writeS(f, "Hello, World!");
		}

		int r;
		std::string s = readS(f, &r);

		if(!active)
		{
			w = writeS(f, s);
		}

		log->MXT_LOG("read \"%s\" (%i -> %i)", s.c_str(), r, w);

		close(f);
	}

	void basicSerialA(void) { basicSerial(true); }
	void basicSerialB(void) { basicSerial(false); }

	void serial(void)
	{
		lib::Thread s1(test::basicSerialB);

		lib::Time::ms(500).wait();

		lib::Thread s2(test::basicSerialA);

		s1.join();
		s2.join();
	}
}
