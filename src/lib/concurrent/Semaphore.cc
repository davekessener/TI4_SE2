#include "Semaphore.h"

// TODO error handling

namespace lib
{

Semaphore::Semaphore(unsigned v) : val_(v)
{
	sem_init(&sem_, 0, v);
}

Semaphore::~Semaphore(void)
{
	sem_destroy(&sem_);
}

void Semaphore::up(void)
{
	sem_post(&sem_);
	++val_;
}

void Semaphore::down(void)
{
	sem_wait(&sem_);
	--val_;
}

}

