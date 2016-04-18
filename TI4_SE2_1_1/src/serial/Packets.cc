#include <cstring>

#include "serial/Packets.h"

namespace hw
{
	DataPacket::DataPacket(const uint8_t *d, uint32_t s)
	{
		size_ = s;
		data_ = s ? new uint8_t[size_] : NULL;

		if(data_) memcpy(data_, d, s);
	}

	void DataPacket::set(uint8_t *d, size_t s)
	{
		delete[] data_;

		data_ = d;
		size_ = s;
	}
}

