#ifndef DAV_MPL_MAP_H
#define DAV_MPL_MAP_H

#include <mpl/list.hpp>

namespace lib
{
	template<int IDX, typename List>
	struct ListToMapImpl
	{
		typedef Cons
		<
			Cons<Int<IDX>, DO(Car<List>)>,
			DO(ListToMapImpl<IDX + 1, DO(Cdr<List>)>)
		> Type;
	};
	
	template<int IDX>
	struct ListToMapImpl<IDX, Nil>
	{
		typedef Nil Type;
	};
	
	template<typename List>
	struct ListToMap
	{
		typedef DO(ListToMapImpl<0, List>) Type;
	};

// ---------------------------------------------------------------------------

	template<typename Map, typename Key>
	struct GetValue
	{
		typedef typename If
		<
			IsSame<DO(Caar<Map>), Key>::value,
			Identity<DO(Cdar<Map>)>,
			GetValue<DO(Cdr<Map>), Key>
		>::Type
		Type;
	};
}

#endif

