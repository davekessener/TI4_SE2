#ifndef HAW_LIB_SINGLETON_H
#define HAW_LIB_SINGLETON_H

#include <stddef.h>

#include <lib/concurrent/Mutex.h>
#include <lib/CleanupUtility.h>

namespace lib
{
/** Contains threading models of the Singleton template
 */
	namespace SingletonConcurrency
	{
		template<typename T>
		struct SingleThreaded
		{
			struct Lock { Lock(Mutex *) { } };
		};

		template<typename T>
		struct MultiThreaded : public LockableClass<T>
		{
			struct Lock
			{
				Lock(Mutex *mtx) : mtx_(mtx) { mtx_->lock(); }
				~Lock( ) { mtx_->unlock(); }
				Mutex *mtx_;
			};
		};
	}

/**
 * Template for convenient Singleton creation.
 * Parameters are:
 * - <b><tt>T</tt></b>: Singleton class
 * - <b><tt>TM</tt></b>: Threading model that will be applied to the singletons creation
 * - <b><tt>P</tt></b>: Priority of the singletons lifetime.
 * This template uses the lib::CleanupUtility to manage its life time.
 */
	template
	<
		typename T,
		template <typename> class TM = SingletonConcurrency::SingleThreaded,
		size_t P = CleanupUtility::DEFAULT_PRIORITY
	>
	class Singleton
	{
		typedef TM<T> ThreadingModel;
		typedef typename ThreadingModel::Lock Lock;

		public:
			static T& instance( );

		private:
			static void destroy_singleton( );

		private:
			static T *instance_;
			static Mutex mtx_;

		private:
			Singleton( );
			Singleton(const Singleton<T, TM, P>&);
			~Singleton( );
			Singleton<T, TM, P>& operator=(const Singleton<T, TM, P>&);
	};

	template<typename T, template <typename> class TM, size_t P>
	T *Singleton<T, TM, P>::instance_ = NULL;

	template<typename T, template <typename> class TM, size_t P>
	Mutex Singleton<T, TM, P>::mtx_;

/** Access singleton class implementation.
 * Uses the <i>double checked locking</i> pattern for
 * creation synchronization.
 */
	template<typename T, template <typename> class TM, size_t P>
	T& Singleton<T, TM, P>::instance(void)
	{
		if(!instance_)
		{
			Lock lock(&mtx_);

			if(!instance_)
			{
				instance_ = new T;
				CleanupUtility::instance().scheduleAtExitWithPriority(destroy_singleton, P);
			}
		}

		return *instance_;
	}

	template<typename T, template <typename> class TM, size_t P>
	void Singleton<T, TM, P>::destroy_singleton(void)
	{
		delete instance_;
	}
}

#endif

