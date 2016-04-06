#ifndef HAW_LIB_SEMAPHORE_H
#define HAW_LIB_SEMAPHORE_H

#include <pthread.h>

namespace lib
{
	class Semaphore
	{
		public:
			Semaphore(unsigned = 0);
			~Semaphore( );
			void up( );
			void down( );
			unsigned get( ) const { return val_; }
		private:
			pthread_mutex_t mtx_;
			pthread_cond_t cond_;
			unsigned val_;
	};
}

#endif

