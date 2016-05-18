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
		//! Empty String
		std::string msg0("");
		//! String filled with multiple NUL
        std::string msg1("");
		for(unsigned int i=0; i<=10; i++)
		{
			unsigned char c = 0;
			msg6.push_back(c);
		}
		//! String with single char
		std::string msg2("ß");
		//! String with multiple characters
		std::string msg3("Hello");
		//! String starting with space
		std::string msg4(" World!");
		//! String with a whole sentence
		std::string msg5("Hello, World!");
		//! String with all ASCII chars
		std::string msg6("");
		for(unsigned char c=33; c<=127; c++)
			msg6.push_back(c);
		//! Long String with all ASCII chars
		std::string msg7("");
		for(unsigned char i=0; i<=255; i++)
			for(unsigned char c=32; c<=127; c++)
				msg7.push_back(c);
		//! String special chars except for NUL	
		std::string msg8("");
		for(unsigned char c=1; c<=33; c++)
			msg8.push_back(c);
		//! Extended ASCII Table Test
		std::string msg9("");
		for(unsigned char c=128; c<=255; c++)
			msg8.push_back(c);
		
		while(!c.connected()) Time::ms(100).wait();


		if(active) 
			c.sendData(Data::get(msg0.c_str(), msg0.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg0.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg1.c_str(), msg1.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg1.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg2.c_str(), msg2.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg2.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg3.c_str(), msg3.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg3.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg4.c_str(), msg4.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg4.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg5.c_str(), msg5.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg5.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg6.c_str(), msg6.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg6.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg7.c_str(), msg7.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg7.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg8.c_str(), msg8.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg8.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");
		
		if(active) 
			c.sendData(Data::get(msg9.c_str(), msg9.length() + 1));

		Data_ptr p = c.receiveData();

		if(!active) 
			c.sendData(p);
		
		log->MXT_LOG("received string \"%s\"", (const char *) p->data());
		
		if(msg9.compare((const char *) p->data()) != 0)
			log->MXT_LOGL(LogLevel::WARNING, "[Send and received Strings are not equal]");
		else
			log->MXT_LOG("Received String is equal to sent String");


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
