#ifndef LIB_HAW_MUTEX_H
#define LIB_HAW_MUTEX_H

#include <pthread.h>

namespace lib
{
	class Mutex
	{
		public:
			Mutex( );
			~Mutex( );
			void lock( );
			void unlock( );
		private:
			pthread_mutex_t mtx_;
		private:
			Mutex(const Mutex&);
			Mutex& operator=(const Mutex&);
	};
}

#endif

