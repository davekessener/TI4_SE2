#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME ListTest
#endif

#include <test/unit_test.hpp>
#include <mpl/list.hpp>
#include <mpl/types.hpp>

#define DC(n) class n { }
DC(A);
DC(B);
DC(C);
DC(D);
#undef DC

BEGIN

using lib::Cons;
using lib::Nil;
using lib::Int;
using lib::Apply;
using lib::IsList;
using lib::Join;
using lib::Flatten;
using lib::Merge;
using lib::Setify;
using lib::Filter;
using lib::ListOr;
using lib::ListAnd;
using lib::True;
using lib::False;
using lib::IsSame;
using lib::GetElem;
using lib::Reverse;

typedef MAKELIST(A, B, C, D) List;
typedef MAKELIST(Int<0>, Int<1>, Int<2>, Int<3>) IntList;


TEST(get_elem)
{
	ASSERT_TRUE((IsSame<C, DO(GetElem<2, List>)>::value));
}

template<typename T>
struct SquareFn
{
	typedef Int<T::value * T::value> Type;
};

TEST(apply)
{
	typedef MAKELIST(Int<0>, Int<1>, Int<4>, Int<9>) SquareList;

	ASSERT_TRUE((IsSame<SquareList, DO(Apply<SquareFn, IntList>)>::value));
}

TEST(reverse)
{
	typedef MAKELIST(D, C, B, A) Expected;

	ASSERT_TRUE((IsSame<Expected, DO(Reverse<List>)>::value));
}

TEST(is_list)
{
	ASSERT_TRUE(( IsList<List>::value), "doesnt recognize a list!");
	ASSERT_TRUE((!IsList<A>::value), "incorrectly recognizes A as a list!");
}

TEST(join)
{
	typedef MAKELIST(A, B, C) PartA;
	typedef MAKELIST(D) PartB;

	ASSERT_TRUE((IsSame<List, DO(Join<PartA, PartB>)>::value));
}

TEST(flatten)
{
	typedef Cons<Cons<Cons<A, Nil>, Nil>, Cons<Cons<B, Cons<Cons<C, Nil>, Cons<D, Nil> > >, Nil> > RawList;

	// Just to compare. c++11:
//	typedef MakeList<MakeList<MakeList<A>>, MakeList<B, MakeList<C>, D>> RawList;

	ASSERT_TRUE((IsSame<List, DO(Flatten<RawList>)>::value));
}

template<typename T1, typename T2>
struct AddFn
{
	typedef Int<T1::value + T2::value> Type;
};

TEST(merge)
{
	ASSERT_TRUE((IsSame<Int<6>, DO(Merge<AddFn, Int<0>, IntList>)>::value));
}

TEST(setify)
{
	typedef MAKELIST(A, A, B, C, A, C, D, A, B) MultiSet;

	ASSERT_TRUE((IsSame<List, DO(Setify<MultiSet>)>::value));
}

template<typename T>
struct FilterFn
{
	static const bool value = T::value % 2 == 0;
};

TEST(filter)
{
	typedef MAKELIST(Int<0>, Int<2>) EvenList;

	ASSERT_TRUE((IsSame<EvenList, DO(Filter<FilterFn, EvenList>)>::value));
}

// TODO TEST(inherit_lineage)

TEST(list_or)
{
	ASSERT_TRUE((!ListOr<MAKELIST(False, False)>::value));
	ASSERT_TRUE(( ListOr<MAKELIST(True,  False)>::value));
	ASSERT_TRUE(( ListOr<MAKELIST(False, True )>::value));
	ASSERT_TRUE(( ListOr<MAKELIST(True,  True )>::value));
}

TEST(list_and)
{
	ASSERT_TRUE((!ListAnd<MAKELIST(False, False)>::value));
	ASSERT_TRUE((!ListAnd<MAKELIST(True,  False)>::value));
	ASSERT_TRUE((!ListAnd<MAKELIST(False, True )>::value));
	ASSERT_TRUE(( ListAnd<MAKELIST(True,  True )>::value));
}

END

