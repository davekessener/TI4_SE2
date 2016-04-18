#ifndef HAW_LIB_SMARTPTR_H
#define HAW_LIB_SMARTPTR_H

#include <stddef.h>
#include <lib/concurrent/Lock.hpp>

namespace lib
{
	template<typename T>
	class SmartPtr : public LockableObject<SmartPtr<T> >
	{
		typedef LockableObject<SmartPtr<T> > Super;
		typedef typename Super::Lock Lock;

		public:
			SmartPtr( ) : p_(NULL), i_(NULL) { }
			explicit SmartPtr(T *p) : p_(p), i_(new int) { *i_ = 1; }
			~SmartPtr( ) { reset(); }
			void reset( ) { Lock guard(this); if(i_ && !*--i_) { delete p_, delete i_; } }
			void set(T *p) { reset(); p_ = p; i_ = new int; *i_ = 1; }
			T *operator->( ) { return p_; }
			const T *operator->( ) const { return p_; }
			T& operator*( ) { return *p_; }
			const T& operator*( ) const { return *p_; }
			template<typename TT>
			TT to( ) { return static_cast<TT>(p_); }
			operator bool() const { return p_; }
		private:
			T *p_;
			int *i_;
	};
}

#endif
