#ifndef DAV_MPL_TYPES_H
#define DAV_MPL_TYPES_H

#include <mpl/cons.hpp>

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

// # ---------------------------------------------------------------------------

	template<typename T>
	struct Decay
	{
		typedef T Type;
	};

	template<typename T>
	struct Decay<const T>
	{
		typedef typename Decay<T>::Type Type;
	};

	template<typename T>
	struct Decay<T&>
	{
		typedef typename Decay<T>::Type Type;
	};

	template<typename T>
	struct Decay<volatile T>
	{
		typedef typename Decay<T>::Type Type;
	};

	template<typename T>
	struct Decay<const volatile T>
	{
		typedef typename Decay<T>::Type Type;
	};

// # ---------------------------------------------------------------------------
	
	template<typename T>
	struct Identity
	{
		typedef T Type;
	};

	template<typename T>
	struct ValueIdentity;

	template<typename T, T I>
	struct ValueIdentity<Value<T, I> >
	{
		static const T value = I;
	};

	template<int I>
	struct ValueIdentity<Int<I> >
	{
		static const int value = I;
	};

	template<bool I>
	struct ValueIdentity<Bool<I> >
	{
		static const int value = I;
	};

// # ---------------------------------------------------------------------------

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

// # ---------------------------------------------------------------------------

	template<typename List, typename T>
	struct Contains
	{
		static const bool value = IsSame<DO(Car<List>), T>::value || Contains<DO(Cdr<List>), T>::value;
	};
	
	template<typename T>
	struct Contains<Nil, T>
	{
		static const bool value = false;
	};

// # ---------------------------------------------------------------------------

	template<typename Sub, typename Super>
	struct IsSuperType
	{
		struct Yes { char v[1]; };
		struct No { char v[2]; };

		template<typename T> static Yes f(T *);
		template<typename T> static No  f(...);

		static const bool value = sizeof(f<Super>(static_cast<Sub *>(NULL))) == sizeof(Yes);
	};
}

#endif

