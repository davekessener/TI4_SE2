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

using lib::MakeList;
using lib::Int;
using lib::DoApply;
using lib::IsList;
using lib::DoJoin;
using lib::DoFlatten;
using lib::DoMerge;
using lib::DoSetify;
using lib::DoFilter;
using lib::ListOr;
using lib::ListAnd;
using lib::True;
using lib::False;
using lib::IsSame;
using lib::DoGetElem;
using lib::DoReverse;

typedef MakeList<A, B, C, D> List;
typedef MakeList<Int<0>, Int<1>, Int<2>, Int<3>> IntList;


TEST(get_elem)
{
	ASSERT_TRUE((IsSame<C, DoGetElem<2, List>>::value));
}

template<typename T>
struct SquareFn
{
	typedef Int<T::value * T::value> Type;
};

TEST(apply)
{
	typedef MakeList<Int<0>, Int<1>, Int<4>, Int<9>> SquareList;

	ASSERT_TRUE((IsSame<SquareList, DoApply<SquareFn, IntList>>::value));
}

TEST(reverse)
{
	typedef MakeList<D, C, B, A> Expected;

	ASSERT_TRUE((IsSame<Expected, DoReverse<List>>::value));
}

TEST(is_list)
{
	ASSERT_TRUE((IsList<List>::value), "doesnt recognize a list!");
	ASSERT_TRUE((!IsList<A>::value), "incorrectly recognizes A as a list!");
}

TEST(join)
{
	typedef MakeList<A, B, C> PartA;
	typedef MakeList<D> PartB;

	ASSERT_TRUE((IsSame<List, DoJoin<PartA, PartB>>::value));
}

TEST(flatten)
{
	typedef MakeList<MakeList<MakeList<A>>, MakeList<B, MakeList<C>, D>> RawList;

	ASSERT_TRUE((IsSame<List, DoFlatten<RawList>>::value));
}

template<typename T1, typename T2>
struct AddFn
{
	typedef Int<T1::value + T2::value> Type;
};

TEST(merge)
{
	ASSERT_TRUE((IsSame<Int<6>, DoMerge<AddFn, Int<0>, IntList>>::value));
}

TEST(setify)
{
	typedef MakeList<A, A, B, C, A, C, D, A, B> MultiSet;

	ASSERT_TRUE((IsSame<List, DoSetify<MultiSet>>::value));
}

template<typename T>
struct FilterFn
{
	static const bool value = T::value % 2 == 0;
};

TEST(filter)
{
	typedef MakeList<Int<0>, Int<2>> EvenList;

	ASSERT_TRUE((IsSame<EvenList, DoFilter<FilterFn, EvenList>>::value));
}

// TODO TEST(inherit_lineage)

TEST(list_or)
{
	ASSERT_TRUE((!ListOr<MakeList<False, False>>::value));
	ASSERT_TRUE(( ListOr<MakeList<True,  False>>::value));
	ASSERT_TRUE(( ListOr<MakeList<False, True >>::value));
	ASSERT_TRUE(( ListOr<MakeList<True,  True >>::value));
}

TEST(list_and)
{
	ASSERT_TRUE((!ListAnd<MakeList<False, False>>::value));
	ASSERT_TRUE((!ListAnd<MakeList<True,  False>>::value));
	ASSERT_TRUE((!ListAnd<MakeList<False, True >>::value));
	ASSERT_TRUE(( ListAnd<MakeList<True,  True >>::value));
}

END

