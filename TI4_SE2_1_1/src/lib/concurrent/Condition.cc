#include <errno.h>
#include <time.h>
#include <stddef.h>

#include "lib/concurrent/Condition.h"
#include "lib/macro.h"

namespace lib
{

Condition::Condition(void) : locked_(false)
{
	pthread_cond_init(&cond_, NULL);
}

Condition::~Condition(void)
{
	pthread_cond_destroy(&cond_);
}

void Condition::wait(void)
{
	if(!locked_)
		MXT_TODO_ERROR; //TODO
	
	pthread_cond_wait(&cond_, &mtx_.raw());
}

bool Condition::wait(timespec *ts)
{
	if(!locked_)
		MXT_TODO_ERROR; //TODO
	
	int r = pthread_cond_timedwait(&cond_, &mtx_.raw(), ts);
	bool success = r == ETIMEDOUT || r == EINVAL;

	if(r && !success)
		MXT_TODO_ERROR; //TODO
	
	return success;
}

void Condition::broadcast(void)
{
	if(!locked_)
		MXT_TODO_ERROR; // TODO
	
	pthread_cond_broadcast(&cond_);
}

void Condition::lock(void)
{
	mtx_.lock();
	locked_ = true;
}

void Condition::unlock(void)
{
	if(!locked_)
		MXT_TODO_ERROR; // TODO
	
	locked_ = false;
	mtx_.unlock();
}

}

