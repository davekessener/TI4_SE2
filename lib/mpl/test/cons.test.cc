#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME ConsTest
#endif

#include <test/unit_test.hpp>
#include <mpl/cons.hpp>
#include <mpl/types.hpp>

#define DC(n) class n { }
DC(A);
DC(B);
DC(C);
DC(D);
#undef DC

BEGIN

using lib::Nil;
using lib::Cons;
using lib::MakeList;
using lib::IsSame;

TEST(make_list)
{
	typedef Cons<A, Cons<B, Cons<C, Cons<D, Nil>>>> Expected;
	typedef MakeList<A, B, C, D> Result;

	ASSERT_TRUE((IsSame<Expected, Result>::value));
}

END

