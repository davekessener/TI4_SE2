#ifndef HAW_LIB_SINGLETON_H
#define HAW_LIB_SINGLETON_H

#include <concurrent/Mutex.h>
#include <CleanupUtility.h>

namespace lib
{
	namespace SingletonConcurrency
	{
		template<typename T>
		struct SingleThreaded
		{
			typedef T *Type;

			struct Lock { Lock(T *) { } };
		};

		template<typename T>
		struct MultiThreaded : public LockableClass<T>
		{
			typedef volatile T *Type;

			using typename LockableClass<T>::Lock;
		};
	}

	template
	<
		typename T,
		template <typename> class ThreadingModel = SingletonConcurrency::SingleThreaded,
		size_t P = CleanupUtility::DEFAULT_PRIORITY
	>
	class Singleton : public ThreadingModel<Singleton<T, ThreadingModel, P>>
	{
		typedef ThreadingModel<Singleton<T, ThreadingModel, P>> Super;
		typedef typename Super::Type Type;
		typedef typename Super::Lock Lock;

		public:
			static T& instance( );

		private:
			static void destroy_singleton( );

		private:
			static Type instance_;

		private:
			Singleton( );
			Singleton(const Singleton<T, P>&);
			~Singleton( );
			Singleton<T, P>& operator=(const Singleton<T, P>&);
	};

	template<typename T, template <typename> class TM, size_t P>
	typename Singleton<T, TM, P>::Type Singleton<T, TM, P>::instance_ = NULL;

	template<typename T, template <typename> class TM, size_t P>
	T& Singleton<T, TM, P>::instance(void)
	{
		if(!instance_)
		{
			Lock lock(NULL);

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

