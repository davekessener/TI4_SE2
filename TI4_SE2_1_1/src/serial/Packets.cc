#include <cstring>

#include "serial/Packets.h"

namespace hw
{
	DataPacket::DataPacket(const uint8_t *d, uint32_t s)
	{
		size_ = s;
		data_ = new uint8_t[size_];

		memcpy(data_, d, s);
	}
}

