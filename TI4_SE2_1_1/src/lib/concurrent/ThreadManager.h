#ifndef HAW_LIB_CONCURRENT_THREADMANAGER_H
#define HAW_LIB_CONCURRENT_THREADMANAGER_H

#include <map>
#include <stdexcept>
#include <stdint.h>
#include <pthread.h>

#include "lib/concurrent/Lock.hpp"
#include "lib/Singleton.hpp"

namespace lib
{
	class ThreadManagerImpl : public LockableObject<ThreadManagerImpl>
	{
		typedef LockableObject<ThreadManagerImpl> Super;
		typedef Super::Lock Lock;

		public:
			typedef uint16_t tid_t;

			tid_t addThread(pthread_t);
			void removeThread(pthread_t);
			tid_t getThread(pthread_t);
			tid_t getCurrent( ) { return getThread(pthread_self()); }

		private:
			std::map<pthread_t, tid_t> ids_;
			tid_t nid_;

		public:
			ThreadManagerImpl( );
			~ThreadManagerImpl( ) { }
		private:
			ThreadManagerImpl(const ThreadManagerImpl&);
			ThreadManagerImpl& operator=(const ThreadManagerImpl&);
	};

	typedef Singleton
	<
		ThreadManagerImpl,
		SingletonConcurrency::MultiThreaded,
		CleanupUtility::DEFAULT_PRIORITY + 1
	>
	ThreadManager;
}

#endif

