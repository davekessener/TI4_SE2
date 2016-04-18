#include "lib/concurrent/Mutex.h"

namespace lib
{

Mutex::Mutex(void)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mtx_, &attr);
}

Mutex::~Mutex(void)
{
	pthread_mutex_destroy(&mtx_);
}

void Mutex::lock(void)
{
	pthread_mutex_lock(&mtx_);
}

void Mutex::unlock(void)
{
	pthread_mutex_unlock(&mtx_);
}

}

