/***************************************************************************
*    Embedded Programming HAW Hamburg Department Informatik
*    HAW:: Thread.cpp
*    COPYRIGHT (C) 2016 Stephan Pareigis
*    This software is licensed under MIT license.                                                                   
****************************************************************************/

#include <exception>

#include "Thread.h"


namespace HAW
{

Thread::Thread(void)
	: joinable_(false)
{
}

Thread::Thread(Thread& t)
	: joinable_(false)
{
	*this = t;
}

Thread::~Thread(void)
{
	if(joinable())
		std::terminate();
}

Thread& Thread::operator=(Thread& t)
{
	if(joinable())
		std::terminate();
	
	if(t.joinable())
	{
		tid_ = t.tid_;
		joinable_ = true;
		t.joinable_ = false;
	}

	return *this;
}

void Thread::join(void)
{
	if(!joinable())
		throw std::runtime_error("Thread cannot be joined.");
	
	pthread_join(tid_ , NULL);
	joinable_ = false;
}

void Thread::run(void)
{
	try
	{
		(*f_)();
	}
	catch(...)
	{
		std::terminate();
	}
}

void *Thread::entryPoint(void *pthis)
{
	Thread *pt = (Thread *) pthis;

	pt->run();

	return NULL;
}

}

