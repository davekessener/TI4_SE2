#include <HWAccess.h>
#include "Lights.h"

#define MXT_REGISTER_ADDR 0x300

namespace SE
{

Lights& Lights::instance(void)
{
	static Lights inst;

	return inst;
}

void Lights::turnOn(std::uint16_t bitmask) const
{
	lib::HWAccess::instance().setBits(MXT_REGISTER_ADDR, bitmask);
}

void Lights::turnOff(std::uint16_t bitmask) const
{
	lib::HWAccess::instance().resetBits(MXT_REGISTER_ADDR, bitmask);
}

}

