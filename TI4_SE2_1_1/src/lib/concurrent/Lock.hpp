#ifndef LIB_HAW_LOCK_H
#define LIB_HAW_LOCK_H

#include <concurrent/Mutex.h>

namespace lib
{
	template
	<
		typename T, 
		void (T::*E)(void) = &T::lock, 
		void (T::*R)(void) = &T::unlock
	>
	class Lock
	{
		public:
			typedef T Mutex;

		public:
			Lock(Mutex& mtx) : mtx_(&mtx) { (mtx_->*E)(); }
			~Lock( ) { (mtx_->*R)(); }
		private:
			Mutex *mtx_;
	};

// # ---------------------------------------------------------------------------

	template<typename T, typename M = Mutex>
	class LockableClass
	{
		public:
			typedef M Mutex;

			struct Lock
			{
				Lock(T *) { mtx_.lock(); }
				~Lock( ) { mtx_.unlock(); }
			};
		private:
			static Mutex mtx_;
	};

	template<typename T, typename M>
	typename LockableClass<T, M>::Mutex LockableClass<T, M>::mtx_;

// # ---------------------------------------------------------------------------

	template<typename T, typename M = Mutex>
	class LockableObject
	{
		public:
			typedef M Mutex;

			struct Lock
			{
				Lock(T *t) : t_(t) { t_->mtx_.lock(); }
				~Lock( ) { t_->mtx_.unlock(); }

				private:
					T *t_;
			};

		private:
			Mutex mtx_;

			friend class Lock;
	};
}

#endif

