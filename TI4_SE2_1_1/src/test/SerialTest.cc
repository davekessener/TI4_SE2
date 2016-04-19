#include "test/SerialTest.h"
#include "serial/Connection.h"

#define MXT_DEVICENAME "/dev/ser1"

namespace test
{
	using hw::Connection;
	using hw::Packet_ptr;

	void serialA(void)
	{
		Connection c(MXT_DEVICENAME, true);
		std::string s("Hello, World!");

		c.write(s.c_str(), s.length());

		c.close();
	}

	void serialB(void)
	{
		Connection c(MXT_DEVICENAME, false);

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
