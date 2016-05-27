#include "hw/Switch.h"
#include "hw/HWAccess.h"

namespace hw {

void Switch::setState(uint8_t s)
{
	(HWAccess::instance().*(s ? &HWAccessImpl::setBits : HWAccessImpl::resetBits))(HWAccessImpl::PORT_A, CLOSED);
}

}

