#ifndef DAV_MPL_LIST_H
#define DAV_MPL_LIST_H

#include "cons.hpp"
#include "types.hpp"
#include "logic.hpp"

namespace lib
{
	template<int IDX, typename List>
	struct GetElem
	{
		typedef typename GetElem<IDX - 1, Cdr<List>>::Type Type;
	};
	
	template<typename List>
	struct GetElem<0, List>
	{
		typedef Car<List> Type;
	};
	
	template<int IDX, typename List>
	using DoGetElem = typename GetElem<IDX, List>::Type;

// ---------------------------------------------------------------------------

	template<template <typename> class F, typename T>
	struct Apply
	{
		typedef Cons<typename F<Car<T>>::Type, typename Apply<F, Cdr<T>>::Type> Type;
	};
	
	template<template <typename> class F>
	struct Apply<F, Nil>
	{
		typedef Nil Type;
	};
	
	template<template <typename> class F, typename T>
	using DoApply = typename Apply<F, T>::Type;

// ---------------------------------------------------------------------------
	
	template<typename Done, typename ToDo>
	struct ReverseImpl
	{
		typedef typename ReverseImpl
		<
			Cons<Car<ToDo>, Done>,
			Cdr<ToDo>
		>::Type
		Type;
	};
	
	template<typename Done>
	struct ReverseImpl<Done, Nil>
	{
		typedef Done Type;
	};
	
	template<typename List>
	struct Reverse
	{
		typedef typename ReverseImpl<Nil, List>::Type Type;
	};
	
	template<typename List>
	using DoReverse = typename Reverse<List>::Type;
	
// ---------------------------------------------------------------------------
	
	template<typename Cell>
	struct ReverseCons
	{
		typedef Cons<Cdr<Cell>, Car<Cell>> Type;
	};
	
	template<typename Cell>
	using DoReverseCons = typename ReverseCons<Cell>::Type;
	
// ---------------------------------------------------------------------------
	
	template<typename T>
	struct IsList
	{
		static const bool value = false;
	};
	
	template<typename T1, typename T2>
	struct IsList<Cons<T1, T2>>
	{
		static const bool value = true;
	};
	
// ---------------------------------------------------------------------------
	
	template<typename List, typename Appendage>
	struct Append
	{
		typedef Cons<Car<List>, typename Append<Cdr<List>, Appendage>::Type> Type;
	};
	
	template<typename Appendage>
	struct Append<Nil, Appendage>
	{
		typedef Appendage Type;
	};
	
	template<typename List, typename Appendage>
	using DoAppend = typename Append<List, Appendage>::Type;
	
// ---------------------------------------------------------------------------
	
	template<typename T>
	struct Flatten
	{
		typedef T Type;
	};
	
	template<typename H, typename T>
	struct Flatten<Cons<H, T>>
	{
		typedef typename Flatten<T>::Type Rest;
	
		typedef DoIf
		<
			IsList<H>,
			Append<typename Flatten<H>::Type, Rest>,
			Identity<Cons<H, Rest>>
		>
		Type;
	};
	
	template<typename List>
	using DoFlatten = typename Flatten<List>::Type;
	
// ---------------------------------------------------------------------------
	
	template<template <typename, typename> class F, typename T, typename List>
	struct Merge
	{
		typedef typename Merge<F, typename F<T, Car<List>>::Type, Cdr<List>>::Type Type;
	};
	
	template<template <typename, typename> class F, typename T>
	struct Merge<F, T, Nil>
	{
		typedef T Type;
	};
	
	template<template <typename, typename> class F, typename T, typename List>
	using DoMerge = typename Merge<F, T, List>::Type;

// ---------------------------------------------------------------------------
	
	template<typename Done, typename ToDo>
	struct SetifyImpl
	{
		typedef typename SetifyImpl
		<
			DoIf
			<
				Contains<Done, Car<ToDo>>,
				Identity<Done>,
				Identity<Cons<Car<ToDo>, Done>>
			>,
			Cdr<ToDo>
		>::Type
		Type;
	};
	
	template<typename Done>
	struct SetifyImpl<Done, Nil>
	{
		typedef DoReverse<Done> Type;
	};
	
	template<typename List>
	struct Setify
	{
		typedef typename SetifyImpl<Nil, List>::Type Type;
	};
	
	template<typename List>
	using DoSetify = typename Setify<List>::Type;
	
// ---------------------------------------------------------------------------
	
	template<template <typename> class F, typename List>
	struct Filter
	{
		typedef typename Filter<F, Cdr<List>>::Type Rest;
	
		typedef DoIf
		<
			F<Car<List>>,
			Identity<Cons<Car<List>, Rest>>,
			Identity<Rest>
		>
		Type;
	};
	
	template<template <typename> class F>
	struct Filter<F, Nil>
	{
		typedef Nil Type;
	};
	
	template<template <typename> class F, typename List>
	using DoFilter = typename Filter<F, List>::Type;

// ---------------------------------------------------------------------------

	template<typename T>
	struct InheritLineage : public Car<T>, public InheritLineage<Cdr<T>>
	{
	};
	
	template<>
	struct InheritLineage<Nil>
	{
	};
	
	template<typename ... T>
	using DoInheritLineage = InheritLineage<MakeList<T...>>;

// ---------------------------------------------------------------------------

	template<typename List>
	struct ListOr
	{
		template<typename T1, typename T2>
		struct OrFn
		{
			typedef Or<T1, T2> Type;
		};
	
		typedef typename Merge<OrFn, False, List>::Type Type;
		static const bool value = Type::value;
	};

	template<typename List>
	struct ListAnd
	{
		template<typename T1, typename T2>
		struct AndFn
		{
			typedef And<T1, T2> Type;
		};
	
		typedef typename Merge<AndFn, True, List>::Type Type;
		static const bool value = Type::value;
	};
}

#endif

