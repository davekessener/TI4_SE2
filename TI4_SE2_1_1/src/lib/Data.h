#ifndef HAW_LIB_DATA_H
#define HAW_LIB_DATA_H

#include <stdint.h>

#include "lib/SmartPtr.hpp"

namespace lib
{
	class Data
	{
		public:
			typedef lib::SmartPtr<Data> Data_ptr;

			~Data( ) { delete[] static_cast<uint8_t *>(data_); }

			static Data_ptr get(const void *, size_t);
			static Data_ptr move(void *d, size_t s) { return Data_ptr(new Data(d, s)); }
			template<typename T>
				static Data_ptr get(const T& t) { return get(&t, sizeof(t)); }
			static Data_ptr empty(size_t s) { return move(new uint8_t[s], s); }

			void *data( ) { return data_; }
			const void *data( ) const { return data_; }
			size_t size( ) const { return size_; }

		private:
			void *data_;
			size_t size_;

		private:
			Data(void *p, size_t s) : data_(p), size_(s) { }

			Data(const Data&);
			Data& operator=(const Data&);
	};

	typedef Data::Data_ptr Data_ptr;
}

#endif

