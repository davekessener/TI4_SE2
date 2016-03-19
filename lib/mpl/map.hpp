#ifndef DAV_MPL_MAP_H
#define DAV_MPL_MAP_H

#include "list.hpp"

namespace lib
{
	template<int IDX, typename List>
	struct ListToMapImpl
	{
		typedef Cons
		<
			Cons<Int<IDX>, Car<List>>,
			typename ListToMapImpl<IDX + 1, Cdr<List>>::Type
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
		typedef typename ListToMapImpl<0, List>::Type Type;
	};
	
	template<typename List>
	using DoListToMap = typename ListToMap<List>::Type;

// ---------------------------------------------------------------------------

	template<typename Map, typename Key>
	struct GetValue
	{
		typedef DoIf
		<
			IsSame<Caar<Map>, Key>,
			Identity<Cdar<Map>>,
			GetValue<Cdr<Map>, Key>
		> Type;
	};
	
	template<typename Map, typename Key>
	using DoGetValue = typename GetValue<Map, Key>::Type;
}

#endif

