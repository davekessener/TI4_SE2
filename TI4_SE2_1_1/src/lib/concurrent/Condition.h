#ifndef HAW_LIB_CONCURRENT_CONDITION_H
#define HAW_LIB_CONCURRENT_CONDITION_H

#include <pthread.h>

#include <lib/concurrent/Mutex.h>
#include <lib/TimeP.h>

namespace lib
{
	class Condition
	{
		public:
			Condition( );
			~Condition( );
			void wait( );
			bool wait(timespec *);
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

