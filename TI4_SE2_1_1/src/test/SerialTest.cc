#include "test/SerialTest.h"
#include "serial/Connection.h"
#include "lib/TimeP.h"

namespace test
{
	using hw::Connection;
	using hw::Packet_ptr;

	void serialA(void)
	{
		lib::Time::ms(100).wait();
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
}
