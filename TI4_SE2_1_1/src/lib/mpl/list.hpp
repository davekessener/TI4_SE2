#ifndef DAV_MPL_LIST_H
#define DAV_MPL_LIST_H

#include <lib/mpl/cons.hpp>
#include <lib/mpl/types.hpp>
#include <lib/mpl/logic.hpp>

namespace lib
{
	template<int IDX, typename List>
	struct GetElem
	{
		typedef DO(GetElem<IDX - 1, DO(Cdr<List>)>) Type;
	};
	
	template<typename List>
	struct GetElem<0, List>
	{
		typedef DO(Car<List>) Type;
	};

// ---------------------------------------------------------------------------

	template<template <typename> class F, typename T>
	struct Apply
	{
		typedef Cons<DO(F<DO(Car<T>)>), DO(Apply<F, DO(Cdr<T>)>)> Type;
	};
	
	template<template <typename> class F>
	struct Apply<F, Nil>
	{
		typedef Nil Type;
	};

// ---------------------------------------------------------------------------
	
	template<typename Done, typename ToDo>
	struct ReverseImpl
	{
		typedef typename ReverseImpl
		<
			Cons<DO(Car<ToDo>), Done>,
			DO(Cdr<ToDo>)
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
		typedef DO(ReverseImpl<Nil, List>) Type;
	};
	
// ---------------------------------------------------------------------------
	
	template<typename Cell>
	struct ReverseCons
	{
		typedef Cons<DO(Cdr<Cell>), DO(Car<Cell>)> Type;
	};
	
// ---------------------------------------------------------------------------
	
	template<typename T>
	struct IsList
	{
		static const bool value = false;
	};
	
	template<typename T1, typename T2>
	struct IsList<Cons<T1, T2> >
	{
		static const bool value = true;
	};
	
// ---------------------------------------------------------------------------
	
	template<typename List, typename Appendage>
	struct Join
	{
		typedef Cons<DO(Car<List>), DO(Join<DO(Cdr<List>), Appendage>)> Type;
	};
	
	template<typename Appendage>
	struct Join<Nil, Appendage>
	{
		typedef Appendage Type;
	};
	
// ---------------------------------------------------------------------------
	
	template<typename T>
	struct Flatten
	{
		typedef T Type;
	};
	
	template<typename H, typename T>
	struct Flatten<Cons<H, T> >
	{
		typedef DO(Flatten<T>) Rest;
	
		typedef typename If
		<
			IsList<H>::value,
			Join<DO(Flatten<H>), Rest>,
			Identity<Cons<H, Rest> >
		>::Type
		Type;
	};
	
// ---------------------------------------------------------------------------
	
	template<template <typename, typename> class F, typename T, typename List>
	struct Merge
	{
		typedef DO(Merge<F, DO(F<T, DO(Car<List>)>), DO(Cdr<List>)>) Type;
	};
	
	template<template <typename, typename> class F, typename T>
	struct Merge<F, T, Nil>
	{
		typedef T Type;
	};

// ---------------------------------------------------------------------------
	
	template<typename Done, typename ToDo>
	struct SetifyImpl
	{
		typedef typename SetifyImpl
		<
			typename If
			<
				Contains<Done, DO(Car<ToDo>)>::value,
				Identity<Done>,
				Identity<Cons<DO(Car<ToDo>), Done> >
			>::Type,
			DO(Cdr<ToDo>)
		>::Type
		Type;
	};
	
	template<typename Done>
	struct SetifyImpl<Done, Nil>
	{
		typedef DO(Reverse<Done>) Type;
	};
	
	template<typename List>
	struct Setify
	{
		typedef DO(SetifyImpl<Nil, List>) Type;
	};
	
// ---------------------------------------------------------------------------
	
	template<template <typename> class F, typename List>
	struct Filter
	{
		typedef DO(Filter<F, DO(Cdr<List>)>) Rest;
	
		typedef typename If
		<
			F<DO(Car<List>)>::value,
			Identity<Cons<DO(Car<List>), Rest> >,
			Identity<Rest>
		>::Type
		Type;
	};
	
	template<template <typename> class F>
	struct Filter<F, Nil>
	{
		typedef Nil Type;
	};

// ---------------------------------------------------------------------------

	template<typename T>
	struct InheritLineage : public Car<T>::Type, public InheritLineage<DO(Cdr<T>)>
	{
	};
	
	template<>
	struct InheritLineage<Nil>
	{
	};

// ---------------------------------------------------------------------------

	template<typename List>
	struct ListOr
	{
		template<typename T1, typename T2>
		struct OrFn
		{
			typedef Or<T1, T2> Type;
		};
	
		typedef DO(Merge<OrFn, False, List>) Type;
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
	
		typedef DO(Merge<AndFn, True, List>) Type;
		static const bool value = Type::value;
	};
}

#endif

