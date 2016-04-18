#include "concurrent/Condition.h"
#include "macro.h"

namespace lib
{

Condition::Condition(void) : locked_(false)
{
	pthread_cond_init(&cond_);
}

Condition::~Condition(void)
{
	pthread_cond_destroy(&cond_);
}

void Condition::wait(void)
{
	if(!locked_)
		MXT_TODO_ERROR; //TODO
	
	pthread_cond_wait(&cond_, &mtx_);
}

bool Condition::wait(struct timespec *ts)
{
	if(!locked_)
		MXT_TODO_ERROR; //TODO
	
	int r = pthread_cond_timedwait(&cond_, &mtx_, ts);
	bool success = r == ETIMEDOUT || r == EINVAL;

	if(s && !success)
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

