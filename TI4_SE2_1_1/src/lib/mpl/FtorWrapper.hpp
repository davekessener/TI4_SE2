#ifndef HAW_LIB_MPL_FTORWRAPPER_H
#define HAW_LIB_MPL_FTORWRAPPER_H

namespace lib
{
	/**
	 * A functor that calls an object's member function.
	 */
	template<typename T>
	class FtorWrapper
	{
		public:
			FtorWrapper(T *t, void (T::*f)(void))
				: t_(t), f_(f) { }
			void operator()( ) { (t_->*f_)(); }
		private:
			T *t_;
			void (T::*f_)(void);
	};

	template<typename T>
	FtorWrapper<T> wrapInFtor(T *t, void (T::*f)(void))
	{
		return FtorWrapper<T>(t, f);
	}

// # ---------------------------------------------------------------------------

	struct BasicFunctor
	{
		virtual void operator()( ) = 0;
		virtual ~BasicFunctor( ) { }
	};
	
	/**
	 * Basic functor encapsulating anything callable that takes no arguments.
	 */
	template<typename F>
	class BasicFunctorImpl : public BasicFunctor
	{
		public:
			BasicFunctorImpl(const F& f) : f_(f) { }
			void operator()( ) { f_(); }
		private:
			F f_;
	};
}

#endif

