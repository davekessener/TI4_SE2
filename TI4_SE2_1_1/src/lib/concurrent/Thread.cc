/***************************************************************************
*    Embedded Programming HAW Hamburg Department Informatik
*    lib/concurrent/Thread.cc
****************************************************************************/

#include <exception>

#include "lib/concurrent/Thread.h"


namespace lib
{

Thread::Thread(void)
	: joinable_(false)
{
}

Thread::~Thread(void)
{
	if(joinable())
		std::terminate();
}

void Thread::join(void)
{
	if(!joinable())
		throw std::runtime_error("Thread cannot be joined.");
	
	pthread_join(tid_, NULL);
	joinable_ = false;

	ThreadManager::instance().removeThread(tid_);
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

