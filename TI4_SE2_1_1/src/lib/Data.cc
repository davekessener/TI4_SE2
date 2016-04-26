#include <string.h>

#include "lib/Data.h"

namespace lib
{

Data::Data_ptr Data::get(const void *p, size_t s)
{
	uint8_t *b = new uint8_t[s];

	memcpy(b, p, s);

	return move(b, s);
}

}

