#ifndef DAV_MPL_TYPES_H
#define DAV_MPL_TYPES_H

namespace lib
{
	template<typename T, T I>
	struct Value
	{
		static const T value = I;
	};
	
	template<int I>
	struct Int : public Value<int, I>
	{
	};
	
	template<bool V>
	struct Bool : public Value<bool, V>
	{
	};
	
	template<typename T>
	struct Identity
	{
		typedef T Type;
	};

	template<typename T1, typename T2>
	struct IsSame
	{
		static const bool value = false;
	};
	
	template<typename T>
	struct IsSame<T, T>
	{
		static const bool value = true;
	};

	template<typename List, typename T>
	struct Contains
	{
		static const bool value = IsSame<Car<List>, T>::value || Contains<Cdr<List>, T>::value;
	};
	
	template<typename T>
	struct Contains<Nil, T>
	{
		static const bool value = false;
	};
}

#endif

