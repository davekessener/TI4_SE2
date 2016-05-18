#include <string>

#include "lib/log/LogManager.h"
#include "lib/TimeP.h"
#include "lib/Data.h"
#include "lib/concurrent/Thread.h"

#include "serial/Connection.h"

#include "test/SerialTest.h"

namespace test
{

using lib::log::Logger_ptr;
using lib::log::LogManager;
using lib::log::LogLevel;
using lib::Time;
using lib::Data_ptr;
using lib::Data;
using lib::Thread;
using hw::Connection;

Logger_ptr getLog()
{
	static Logger_ptr log = LogManager::instance().rootLog();

	return log;
}

void connection(const std::string& device, const bool active)
{
	Logger_ptr log = getLog();

	log->MXT_LOG("Starting serial test as %s", (active ? "ACTIVE" : "PASSIVE"));

	try
	{
		Connection c(device, active);
		std::string msg("Hello, World!");


		while(!c.connected()) Time::ms(100).wait();


		if(active) c.sendData(Data::get(msg.c_str(), msg.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) c.sendData(p);


		log->MXT_LOG("received string \"%s\"", (const char *) p->data());


		while(!c.doneWriting()) Time::ms(100).wait();


		if(!active) Time::s(1).wait();


		c.close();
	}
	catch(const std::string& e)
	{
		log->MXT_LOGL(LogLevel::ERROR, "%s [errno %i (%s)]", e.c_str(), errno, strerror(errno));
	}
}

void serialA(void) { connection("/dev/ser1", true); }
void serialB(void) { connection("/dev/ser2", false); }
void connections(void)
{
	//! Creates a serial Connection on serialA
	Thread t1(serialA);
	//! Creates a serial Connection on serialB
	Thread t2(serialB);

	//! Destroys serial Connection on serialA
	t1.join();
	//! Destroys serial Connection on serialB
	t2.join();
}

}
