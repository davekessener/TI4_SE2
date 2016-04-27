#ifndef HAW_LIB_SMARTPTR_H
#define HAW_LIB_SMARTPTR_H

#include <stddef.h>
#include <lib/concurrent/Lock.hpp>

namespace lib
{
/** Smart pointer class for automatic life time management.
 * Supports full object semantics and automatically cleans
 * up when the last SmartPtr instance pointing to its held
 * object is destroyed.
 */
	template<typename T>
	class SmartPtr : public LockableObject<SmartPtr<T> >
	{
		typedef LockableObject<SmartPtr<T> > Super;
		typedef typename Super::Lock Lock;

		public:
			SmartPtr( ) : p_(NULL), i_(NULL) { }
			explicit SmartPtr(T *p) : p_(p), i_(new int) { *i_ = 1; }
			SmartPtr(const SmartPtr<T>& p) : p_(p.p_), i_(p.i_) { ++*i_; }
			~SmartPtr( ) { reset(); }
			SmartPtr<T>& operator=(const SmartPtr<T>& p)
				{ Lock guard(this); reset(); if((p_ = p.p_)) { i_ = p.i_; ++*i_; } return *this; }
			void reset( ) { Lock guard(this); if(i_ && !*--i_) { delete p_, delete i_; } }
			void set(T *p) { Lock giard(this); reset(); if((p_ = p)) { i_ = new int; *i_ = 1; } }
			T *operator->( ) { return p_; }
			const T *operator->( ) const { return p_; }
			T& operator*( ) { return *p_; }
			const T& operator*( ) const { return *p_; }
			template<typename TT>
				TT to( ) { return static_cast<TT>(p_); }
			operator bool() const { return p_; }
			bool operator==(const SmartPtr<T>& p) const { return p_ == p.p_; }
			bool operator!=(const SmartPtr<T>& p) const { return p_ != p.p_; }
		private:
			T *p_;
			int *i_;
	};
}

#endif

//#ifndef HAW_LIB_SMARTPTR_H
//#define HAW_LIB_SMARTPTR_H
//
//#include <concurrent/Lock.hpp>
//
//namespace lib
//{
//	template<typename T>
//	class SmartPtr : public LockableObject<SmartPtr<T> >
//	{
//		typedef LockableObject<SmartPtr<T> > Super;
//		using Super::Lock;
//
//		public:
//			SmartPtr( );
//			explicit SmartPtr(T *);
//			SmartPtr(const SmartPtr<T>&);
//			~SmartPtr( );
//			SmartPtr<T>& operator=(const SmartPtr<T>&);
//			void swap(SmartPtr<T>&);
//			T *get( ) { return t_; }
//			const T *get( ) const { return t_; }
//			void set(T *);
//			void reset( );
//			T *operator->( ) { return t_; }
//			const T *operator->( ) const { return t_; }
//			T& operator*( ) { return *t_; }
//			const T& operator*( ) { return *t_; }
//			explicit operator bool( ) const { return t_; }
//			template<typename TT>
//				explicit operator TT *( ) { return static_cast<TT *>(t_); }
//			template<typename TT>
//				explicit operator const TT *( ) const { return static_cast<const TT *>(t_); }
//		private:
//			T *t_;
//			unsigned *i_;
//	};
//
//	template<typename T>
//	SmartPtr<T>::SmartPtr(void)
//	: t_(NULL), i_(NULL)
//	{
//	}
//
//	template<typename T>
//	SmartPtr<T>::SmartPtr(T *t)
//	: t_(NULL), i_(NULL)
//	{
//		set(t);
//	}
//
//	template<typename T>
//	SmartPtr<T>::SmartPtr(const SmartPtr<T>& p)
//	: t_(NULL), i_(NULL)
//	{
//		*this = p;
//	}
//
//	template<typename T>
//	SmartPtr<T>::~SmartPtr(void)
//	{
//		reset();
//	}
//
//	template<typename T>
//	SmartPtr<T>& operator=(const SmartPtr<T>& p)
//	{
//		Lock lock(this);
//
//		reset();
//		t_ = p.t_;
//		i_ = p.i_;
//		if(i_) ++*i_;
//	}
//
//	template<typename T>
//	void SmartPtr<T>::swap(SmartPtr<T>& p)
//	{
//		SmartPtr<T> t(*this);
//
//		*this = p;
//		
//		p = t;
//	}
//
//	template<typename T>
//	void SmartPtr<T>::set(T *t)
//	{
//		Lock lock(this);
//
//		reset();
//		if((t_ = t))
//		{
//			i_ = new unsigned;
//			*i_ = 1;
//		}
//	}
//
//	template<typename T>
//	void SmartPtr<T>::reset(void)
//	{
//		Lock lock(this);
//
//		if(i_ && !--*i_)
//		{
//			delete t_;
//			delete i_;
//		}
//
//		t_ = NULL;
//		i_ = NULL;
//	}
//}
//
//#endif

