#include <sys/neutrino.h>
#include <hw/inout.h>
#include <stdexcept>

#include "hw/HWAccess.h"

#define MXT_IO_CONTROL_ADDR 0x303
#define MXT_IO_CONTROL_BITMASK 0x8A //10001010

namespace hw
{

HWAccess& HWAccess::instance(void)
{
	static HWAccess inst;

	return inst;
}

HWAccess::HWAccess(void)
{
    if(ThreadCtl(_NTO_TCTL_IO_PRIV, 0))
    	throw std::runtime_error("no access to hw");

	out8(MXT_IO_CONTROL_ADDR, MXT_IO_CONTROL_BITMASK);
}

HWAccess::~HWAccess(void)
{
}

uint8_t HWAccess::in(port_t addr) const
{
	return in8(addr);
}

void HWAccess::out(port_t addr, pin_t v) const
{
	out8(addr, v);
}

void HWAccess::setBits(port_t addr, pin_t mask) const
{
	out(addr, MXT_SETBITS(in(addr), mask));
}

void HWAccess::resetBits(port_t addr, pin_t mask) const
{
	out(addr, MXT_RESETBITS(in(addr), ~mask));
}

}

