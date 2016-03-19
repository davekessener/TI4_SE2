#ifndef DAV_MPL_LOGIC_H
#define DAV_MPL_LOGIC_H

#include "types.hpp"

namespace lib
{
	typedef Bool<true> True;
	typedef Bool<false> False;
	
	template<bool Cond, typename T1, typename T2>
	struct If;
	
	template<typename T1, typename T2>
	struct If<true, T1, T2>
	{
		typedef typename T1::Type Type;
	};
	
	template<typename T1, typename T2>
	struct If<false, T1, T2>
	{
		typedef typename T2::Type Type;
	};
	
	template<typename C, typename T1, typename T2>
	using DoIf = typename If<C::value, T1, T2>::Type;

	template<typename T>
	struct Not
	{
		static const bool value = !T::value;
	};
	
	template<typename T1, typename T2>
	struct And
	{
		static const bool value = T1::value && T2::value;
	};
	
	template<typename T1, typename T2>
	struct Or
	{
		static const bool value = T1::value || T2::value;
	};

/* 
	template<typename T>
	struct HasMember
	{
		template<typename TT, void (TT::*)(void)> // member signature
		struct Check
		{
		};
	
		template<typename TT>
		static True test(Check<TT, &TT::__MEMBER_NAME__>*);
	
		template<typename TT>
		static False test(...);
	
		static const bool value = decltype(test<T>(nullptr))::value;
	};
*/
}

#endif

