#ifndef HAW_LIB_MPL_FTORWRAPPER_H
#define HAW_LIB_MPL_FTORWRAPPER_H

namespace lib
{
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
}

#endif

