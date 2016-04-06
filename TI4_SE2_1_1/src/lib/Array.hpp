#ifndef LIB_HAW_ARRAY_H
#define LIB_HAW_ARRAY_H

#include <cstddef>
#include <stdexcept>

namespace lib
{
	template<typename T, std::size_t N>
	class Array
	{
		public:
			typedef T value_type;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef value_type *pointer;
			typedef const value_type *const_pointer;
			typedef pointer iterator;
			typedef const_pointer const_iterator;

		public:
			reference at(size_type i) { if(i >= size()) throw std::out_of_range("array:at"); return arr_[i]; }
			const_reference at(size_type i) const { if(i >= size()) throw std::out_of_range("array:at const"); return arr_[i]; }
			reference operator[](size_type i) { return arr_[i]; }
			const_reference operator[](size_type i) const { return arr_[i]; }
			reference front( ) { return arr_[0]; }
			const_reference front( ) const { return arr_[0]; }
			reference back( ) { return arr_[N - 1]; }
			const_reference back( ) const { return arr_[N - 1]; }
			pointer data( ) { return arr_; }
			const_pointer data( ) const { return arr_; }

			iterator begin( ) { return arr_; }
			const_iterator cbegin( ) const { return arr_; }
			iterator end( ) { return begin() + N; }
			const_iterator cend( ) { return cbegin() + N; }

			bool empty( ) const { return false; }
			size_type size( ) const { return N; }
			size_type max_size( ) const { return N; }
		private:
			value_type arr_[N];
	};

	template<typename T>
	class Array<T, 0>
	{
	};
}

#endif

