#ifndef HAW_LIB_SEMAPHORE_H
#define HAW_LIB_SEMAPHORE_H

#include <semaphore.h>

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
			sem_t sem_;
			unsigned val_;
	};
}

#endif

