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

using lib::MakeList_3;
using lib::ListToMap;
using lib::GetValue;
using lib::Cons;
using lib::IsSame;
using lib::Int;

typedef MAKELIST(A, B, C) List;
typedef DO(MakeList_3
<
	Cons<Int<0>, A>,
	Cons<Int<1>, B>,
	Cons<Int<2>, C>
>) Map;

TEST(list_to_map)
{
	ASSERT_TRUE((IsSame<Map, DO(ListToMap<List>)>::value));
}

TEST(get_value)
{
	ASSERT_TRUE((IsSame<C, DO(GetValue<Map, Int<2> >)>::value));
}

END

