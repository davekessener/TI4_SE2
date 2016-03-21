#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME MapTest
#endif

#include <test/unit_test.hpp>
#include <mpl/map.hpp>
#include <mpl/cons.hpp>
#include <mpl/types.hpp>

class A { };
class B { };
class C { };

BEGIN

using lib::DoListToMap;
using lib::DoGetValue;
using lib::MakeList;
using lib::Cons;
using lib::IsSame;
using lib::Int;

typedef MakeList<A, B, C> List;
typedef MakeList
<
	Cons<Int<0>, A>,
	Cons<Int<1>, B>,
	Cons<Int<2>, C>
> Map;

TEST(list_to_map)
{
	ASSERT_TRUE((IsSame<Map, DoListToMap<List>>::value));
}

TEST(get_value)
{
	ASSERT_TRUE((IsSame<C, DoGetValue<Map, Int<2>>>::value));
}

END

