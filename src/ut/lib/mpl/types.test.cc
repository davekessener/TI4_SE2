#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME TypesTest
#endif

#include <test/unit_test.hpp>
#include <mpl/types.hpp>
#include <mpl/cons.hpp>

class A { };
class B { };
typedef A C;
class D { };
class E : public D { };

BEGIN

using lib::IsSame;
using lib::Contains;
using lib::IsSuperType;

TEST(is_same)
{
	ASSERT_TRUE(( IsSame<A, A>::value), "equality failure");
	ASSERT_TRUE((!IsSame<A, B>::value), "inequality failure");
	ASSERT_TRUE(( IsSame<A, C>::value), "typedef-equality failure");
}

TEST(contains)
{
	typedef MAKELIST(A, B) List;

	ASSERT_TRUE(( Contains<List, A>::value), "can't find A in (A B)!");
	ASSERT_TRUE((!Contains<List, D>::value), "can find D in (A B)!");
}

TEST(is_supertype)
{
	ASSERT_TRUE(( IsSuperType<E, D>::value), "doesn't recognize inheritance!");
	ASSERT_TRUE((!IsSuperType<D, E>::value), "incorrectly recognizes inverse inheritance!");
	ASSERT_TRUE((!IsSuperType<A, B>::value), "incorrectly recognizes inheritance!");
}

END

