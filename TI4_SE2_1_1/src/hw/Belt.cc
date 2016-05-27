#include "hw/Belt.h"
#include "hw/HWAccess.h"

namespace hw {

void Belt::move(uint8_t dir, uint8_t speed)
{
	stop();

	HWAccess::instance().setBits(HWAccessImpl::PORT_A, dir | speed);
}

void Belt::changeSpeed(uint8_t speed)
{
	(HWAccess::instance().*(speed ? &HWAccessImpl::resetBits : &HWAccessImpl::setBits))(HWAccessImpl::PORT_A, SLOW);
}

void Belt::stop(void)
{
	HWAccess::instance().resetBits(HWAccessImpl::PORT_A, BITMASK);
}

void Belt::doPause(bool f)
{
	(HWAccess::instance().*(f ? &HWAccessImpl::setBits : &HWAccessImpl::resetBits))(HWAccessImpl::PORT_A, PAUSE);
}

}

