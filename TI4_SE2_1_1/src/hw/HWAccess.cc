#include <sys/neutrino.h>
#include <hw/inout.h>
#include <stdexcept>

#include "hw/HWAccess.h"
#include "lib/log/LogManager.h"
#include "lib/macro.h"

#define MXT_IO_CONTROL_ADDR 0x303
#define MXT_IO_CONTROL_BITMASK 0x8A //10001010

namespace hw
{

void HWAccessImpl::initThread(void)
{
	Lock guard(this);

    if(ThreadCtl(_NTO_TCTL_IO_PRIV, 0) < 0)
    	throw std::runtime_error("no access to hw");

	out8(MXT_IO_CONTROL_ADDR, MXT_IO_CONTROL_BITMASK);
}

uint8_t HWAccessImpl::in(port_t addr)
{
	Lock guard(this);

	return in8(addr);
}

void HWAccessImpl::out(port_t addr, pin_t v)
{
	Lock guard(this);

	out8(addr, v);
}

void HWAccessImpl::setBits(port_t addr, pin_t mask)
{
	Lock guard(this);

	out(addr, MXT_SETBITS(in(addr), mask));
}

void HWAccessImpl::resetBits(port_t addr, pin_t mask)
{
	Lock guard(this);

	out(addr, MXT_RESETBITS(in(addr), mask));
}

}

