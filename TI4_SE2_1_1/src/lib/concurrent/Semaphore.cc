#include <stddef.h>

#include "lib/concurrent/Semaphore.h"

// TODO error handling

namespace lib
{

Semaphore::Semaphore(unsigned v) : val_(v)
{
	pthread_mutex_init(&mtx_, NULL);
	pthread_cond_init(&cond_, NULL);
}

Semaphore::~Semaphore(void)
{
	pthread_mutex_destroy(&mtx_);
	pthread_cond_destroy(&cond_);
}

void Semaphore::up(void)
{
	pthread_mutex_lock(&mtx_);
	++val_;
	if(val_ == 1) pthread_cond_broadcast(&cond_);
	pthread_mutex_unlock(&mtx_);
}

void Semaphore::down(void)
{
	pthread_mutex_lock(&mtx_);
	while(!val_)
	{
		pthread_cond_wait(&cond_, &mtx_);
	}
	--val_;
	pthread_mutex_unlock(&mtx_);
}

}

