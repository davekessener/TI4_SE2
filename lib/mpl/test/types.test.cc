#include <test/unit_test.hpp>
#include <mpl/types.hpp>

class A { };
class B { };
typedef A C;

BEGIN

using lib::IsSame;

TEST(test_IsSame_equality)
{
	ASSERT_TRUE((IsSame<A, A>::value));
}

TEST(test_IsSame_inequality)
{
	ASSERT_TRUE((!IsSame<A, B>::value));
}

TEST(test_IsSame_typedef_equality)
{
	ASSERT_TRUE((IsSame<A, C>::value));
}

END

