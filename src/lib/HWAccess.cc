#include <sys/neutrino.h>
#include <hw/inout.h>

#include "HWAccess.h"

#define MXT_IO_CONTROL_ADDR 0x303
#define MXT_IO_CONTROL_BITMASK 0x8A //10001010

namespace lib
{

HWAccess& HWAccess::instance(void)
{
	static HWAccess inst;

	return inst;
}

HWAccess::HWAccess(void)
{
	out8(MXT_IO_CONTROL_ADDR, IO_CONTROL_BITMASK);
}

HWAccess::~HWAccess(void)
{
}

std::uint16_t HWAccess::in(std::uint16_t addr) const
{
	return in8(addr);
}

void HWAccess::out(std::uint16_t addr, std::uint16_t v) const
{
	out8(addr, v);
}

void HWAccess::setBits(std::uint16_t addr, std::uint16_t mask) const
{
	out(addr, in(addr) | mask);
}

void HWAccess::resetBits(std::uint16_t addr, std::uint16_t mask) const
{
	out(addr, in(addr) & ~mask);
}

}

