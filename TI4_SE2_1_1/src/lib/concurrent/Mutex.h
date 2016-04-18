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
			pthread_mutex_t& raw( ) { return mtx_; }
		private:
			pthread_mutex_t mtx_;
	};
}

#endif

