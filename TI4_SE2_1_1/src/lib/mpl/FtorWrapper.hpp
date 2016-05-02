#ifndef HAW_LIB_MPL_FTORWRAPPER_H
#define HAW_LIB_MPL_FTORWRAPPER_H

#include "lib/SmartPtr.hpp"

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

// # ---------------------------------------------------------------------------
	
	template<typename R, typename T>
	struct OPFImplBase
	{
		virtual ~OPFImplBase( ) { }
		virtual R operator()(T) = 0;
	};

	template<typename T>
	struct OPFImplBase<void, T>
	{
		virtual ~OPFImplBase( ) { }
		virtual void operator()(T) = 0;
	};

	template<typename R>
	struct OPFImplBase<R, void>
	{
		virtual ~OPFImplBase( ) { }
		virtual R operator()( ) = 0;
	};

	template<typename F, typename R, typename T>
	class OPFImpl : public OPFImplBase<R, T>
	{
		public:
			OPFImpl(F f) : f_(f) { }
			R operator()(T t) { return (*f_)(t); }
		private:
			F f_;
	};

	template<typename F, typename T>
	class OPFImpl<F, void, T> : public OPFImplBase<void, T>
	{
		public:
			OPFImpl(F f) : f_(f) { }
			void operator()(T t) { (*f_)(t); }
		private:
			F f_;
	};

	template<typename F, typename R>
	class OPFImpl<F, R, void> : public OPFImplBase<R, void>
	{
		public:
			OPFImpl(F f) : f_(f) { }
			R operator()( ) { return (*f_)(); }
		private:
			F f_;
	};

	template<typename TT, typename R, typename T>
	class OPFMImpl : public OPFImplBase<R, T>
	{
		typedef R (TT::*fn_t)(T);

		public:
			OPFMImpl(TT *t, fn_t f) : t_(t), f_(f) { }
			R operator()(T t) { return (t_->*f_)(t); }
		private:
			TT *t_;
			fn_t f_;
	};

	template<typename TT, typename T>
	class OPFMImpl<TT, void, T> : public OPFImplBase<void, T>
	{
		typedef void (TT::*fn_t)(T);

		public:
			OPFMImpl(TT *t, fn_t f) : t_(t), f_(f) { }
			void operator()(T t) { (t_->*f_)(t); }
		private:
			TT *t_;
			fn_t f_;
	};

	template<typename TT, typename R>
	class OPFMImpl<TT, R, void> : public OPFImplBase<R, void>
	{
		typedef R (TT::*fn_t)(void);

		public:
			OPFMImpl(TT *t, fn_t f) : t_(t), f_(f) { }
			R operator()( ) { return (t_->*f_)(); }
		private:
			TT *t_;
			fn_t f_;
	};

	template<typename R, typename T>
	class OneParamFtor
	{
		public:
			OneParamFtor( ) { }
			template<typename F>
				OneParamFtor(F f) : f_(new OPFImpl<F, R, T>(f)) { }
			template<typename TT>
				OneParamFtor(TT *t, R (TT::*f)(T)) : f_(new OPFMImpl<TT, R, T>(t, f)) { }
			R operator()(T t) { return (*f_)(t); }
			operator bool( ) const { return static_cast<bool>(f_); }
		private:
			SmartPtr<OPFImplBase<R, T> > f_;
	};

	template<typename T>
	class OneParamFtor<void, T>
	{
		public:
			OneParamFtor( ) { }
			template<typename F>
				OneParamFtor(F f) : f_(new OPFImpl<F, void, T>(f)) { }
			template<typename TT>
				OneParamFtor(TT *t, void (TT::*f)(T)) : f_(new OPFMImpl<TT, void, T>(t, f)) { }
			void operator()(T t) { (*f_)(t); }
			operator bool( ) const { return static_cast<bool>(f_); }
		private:
			SmartPtr<OPFImplBase<void, T> > f_;
	};

	template<typename R>
	class OneParamFtor<R, void>
	{
		public:
			OneParamFtor( ) { }
			template<typename F>
				OneParamFtor(F f) : f_(new OPFImpl<F, R, void>(f)) { }
			template<typename TT>
				OneParamFtor(TT *t, R (TT::*f)(void)) : f_(new OPFMImpl<TT, R, void>(t, f)) { }
			R operator()( ) { return (*f_)(); }
			operator bool( ) const { return static_cast<bool>(f_); }
		private:
			SmartPtr<OPFImplBase<R, void> > f_;
	};

	template<typename R, typename T>
	OneParamFtor<R, T> toFtor(R (*f)(T))
	{
		return OneParamFtor<R, T>(f);
	}

	template<typename TT, typename R, typename T>
	OneParamFtor<R, T> toFtor(TT *t, R (TT::*f)(T))
	{
		return OneParamFtor<R, T>(t, f);
	}
}

#endif

