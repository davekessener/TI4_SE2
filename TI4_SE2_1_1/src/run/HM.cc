#include "run/HM.h"
#include "hw/Sensor.h"

#define MXT_CSAMP 24

namespace haw
{

using hw::Sensors;
typedef unsigned uint;

void HM::tick(void)
{
	if(!avg_)
		MXT_TODO_ERROR;

	vals_.push_back(Sensors::instance().getHeight());
}

uint32_t HM::stop(void)
{
	if(!avg_)
		MXT_TODO_ERROR;

	if(vals_.size() < MXT_CSAMP)
		MXT_TODO_ERROR; //TODO

	uint16_t samp[MXT_CSAMP];

	uint d = (vals_.size() + MXT_CSAMP - 1) / MXT_CSAMP;
	uint32_t min = -1, max = 0;
	std::vector<uint16_t>::iterator i1 = vals_.begin(), i2 = vals_.end();
	for(uint i = 0 ; i < MXT_CSAMP ; ++i)
	{
		uint t = 0, c = d;
		for(; c && i1 != i2 ; --c, ++i1)
		{
			t += *i1;
			if(*i1 < min) min = *i1;
			if(*i1 > max) max = *i1;
		}
		t /= d - c;

		samp[i] = t;
	}

	vals_.clear();

	uint avg = (min + max) / 2;
	for(uint i = 0 ; i < MXT_CSAMP ; ++i)
	{
		samp[i] = (samp[i] <= avg) ? 0 : 1;
	}

	return max <= avg_ ? Puk::Type::FLAT : (Puk::Type::LARGE | (samp[0] ? Puk::UPSIDE_DOWN : 0));
}

}

