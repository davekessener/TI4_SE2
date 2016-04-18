#ifndef HAW_LIB_CONCURRENT_CONDITION_H
#define HAW_LIB_CONCURRENT_CONDITION_H

#include <pthread.h>

#include <concurrent/Mutex.h>
#include <Time.h>

namespace lib
{
	class Condition
	{
		public:
			Condition( );
			~Condition( );
			void wait( );
			bool wait(struct timespec *);
			void broadcast( );
			void lock( );
			void unlock( );
		private:
			bool locked_;
			Mutex mtx_;
			pthread_cond_t cond_;
	};
}

#endif

