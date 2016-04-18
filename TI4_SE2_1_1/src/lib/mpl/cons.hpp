#ifndef DAV_MPL_CONS_H
#define DAV_MPL_CONS_H

#include <lib/macro.h>

#define MAKELIST_0    ::lib::Nil
#define MAKELIST_1(a) ::lib::Cons<a, MAKELIST_0>
#define MAKELIST_2(a, b) ::lib::Cons<a, MAKELIST_1(b)>
#define MAKELIST_3(a, b, c) ::lib::Cons<a, MAKELIST_2(b, c)>
#define MAKELIST_4(a, b, c, d) ::lib::Cons<a, MAKELIST_3(b, c, d)>
#define MAKELIST_5(a, b, c, d, e) ::lib::Cons<a, MAKELIST_4(b, c, d, e)>
#define MAKELIST_6(a, b, c, d, e, f) ::lib::Cons<a, MAKELIST_5(b, c, d, e, f)>
#define MAKELIST_7(a, b, c, d, e, f, g) ::lib::Cons<a, MAKELIST_6(b, c, d, e, f, g)>
#define MAKELIST_8(a, b, c, d, e, f, g, h) ::lib::Cons<a, MAKELIST_7(b, c, d, e, f, g, h)>
#define MAKELIST_9(a, b, c, d, e, f, g, h, i) ::lib::Cons<a, MAKELIST_8(b, c, d, e, f, g, h, i)>
#define MAKELIST(...) MXT_OVERLOAD(MAKELIST_, __VA_ARGS__)

#define DO(...) typename __VA_ARGS__::Type

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
	
	template<typename T1, typename T2>
	struct ConsFn
	{
		typedef Cons<T1, T2> Type;
	};

	template<typename T>
	struct Car
	{
		typedef typename T::Head Type;
	};

	template<typename T>
	struct Cdr
	{
		typedef typename T::Tail Type;
	};

	template<typename T>
	struct Caar
	{
		typedef DO(Car<DO(Car<T>)>) Type;
	};

	template<typename T>
	struct Cadr
	{
		typedef DO(Car<DO(Cdr<T>)>) Type;
	};

	template<typename T>
	struct Cdar
	{
		typedef DO(Cdr<DO(Car<T>)>) Type;
	};

	template<typename T>
	struct Cddr
	{
		typedef DO(Cdr<DO(Cdr<T>)>) Type;
	};

		struct MakeList_0 { typedef Nil Type; };
	template<typename A> 
		struct MakeList_1 { typedef Cons<A, MAKELIST_0> Type; };
	template<typename A, typename B> 
		struct MakeList_2 { typedef Cons<A, MAKELIST_1(B)> Type; };
	template<typename A, typename B, typename C> 
		struct MakeList_3 { typedef Cons<A, MAKELIST_2(B, C)> Type; };
	template<typename A, typename B, typename C, typename D> 
		struct MakeList_4 { typedef Cons<A, MAKELIST_3(B, C, D)> Type; };
	template<typename A, typename B, typename C, typename D, typename E> 
		struct MakeList_5 { typedef Cons<A, MAKELIST_4(B, C, D, E)> Type; };
	template<typename A, typename B, typename C, typename D, typename E, typename F> 
		struct MakeList_6 { typedef Cons<A, MAKELIST_5(B, C, D, E, F)> Type; };
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G> 
		struct MakeList_7 { typedef Cons<A, MAKELIST_6(B, C, D, E, F, G)> Type; };
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H> 
		struct MakeList_8 { typedef Cons<A, MAKELIST_7(B, C, D, E, F, G, H)> Type; };
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I> 
		struct MakeList_9 { typedef Cons<A, MAKELIST_8(B, C, D, E, F, G, H, I)> Type; };
}

#endif

