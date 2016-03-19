#ifndef DAV_MPL_CONS_H
#define DAV_MPL_CONS_H

namespace lib
{
	struct Nil
	{
		typedef Nil Type;
	};
	
	template<typename H, typename T>
	struct Cons
	{
		typedef H Head;
		typedef T Tail;
	};
	
	template<typename ... T>
	struct ConsMaker;
	
	template<typename T, typename ... TT>
	struct ConsMaker<T, TT...>
	{
		typedef Cons<T, typename ConsMaker<TT...>::Type> Type;
	};
	
	template<>
	struct ConsMaker<>
	{
		typedef Nil Type;
	};
	
	template<typename T1, typename T2>
	struct ConsFn
	{
		typedef Cons<T1, T2> Type;
	};
	
	template<typename ... T>
	using MakeList = typename ConsMaker<T...>::Type;
	
	template<typename T>
	using Car = typename T::Head;
	
	template<typename T>
	struct CarFn
	{
		typedef Car<T> Type;
	};
	
	template<typename T>
	using Cdr = typename T::Tail;
	
	template<typename T>
	struct CdrFn
	{
		typedef Cdr<T> Type;
	};
	
	template<typename T>
	using Caar = Car<Car<T>>;
	
	template<typename T>
	struct CaarFn
	{
		typedef Caar<T> Type;
	};
	
	template<typename T>
	using Cadr = Car<Cdr<T>>;
	
	template<typename T>
	struct CadrFn
	{
		typedef Cadr<T> Type;
	};
	
	template<typename T>
	using Cdar = Cdr<Car<T>>;
	
	template<typename T>
	struct CdarFn
	{
		typedef Cdar<T> Type;
	};
	
	template<typename T>
	using Cddr = Cdr<Cdr<T>>;
	
	template<typename T>
	struct CddrFn
	{
		typedef Cddr<T> Type;
	};
}

#endif

