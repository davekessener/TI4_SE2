#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME LogicTest
#endif

#include <test/unit_test.hpp>
#include <mpl/logic.hpp>
#include <mpl/types.hpp>

class A { };
class B { };

BEGIN

using lib::IsSame;
using lib::If;
using lib::Identity;

TEST(meta_if)
{
	ASSERT_TRUE((IsSame<A, DO(If<true,  Identity<A>, Identity<B> >)>::value));
	ASSERT_TRUE((IsSame<B, DO(If<false, Identity<A>, Identity<B> >)>::value));
}

END

