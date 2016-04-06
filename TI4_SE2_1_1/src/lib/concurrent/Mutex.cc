#include "Mutex.h"

namespace lib
{

Mutex::Mutex(void)
{
	pthread_mutex_init(&mtx_, NULL);
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

