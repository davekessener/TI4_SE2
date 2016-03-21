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
using lib::DoIf;
using lib::True;
using lib::False;
using lib::Identity;

TEST(meta_if)
{
	ASSERT_TRUE((IsSame<A, DoIf<True,  Identity<A>, Identity<B>>>::value));
	ASSERT_TRUE((IsSame<B, DoIf<False, Identity<A>, Identity<B>>>::value));
}

END

