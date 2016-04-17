#include "hw/LED.h"

namespace hw
{

void LED::doTurnOn(led_t led)
{
	HWAccess::instance().setBits(MXT_PORT(led), MXT_PINS(led));
}

void LED::doTurnOff(led_t led)
{
	HWAccess::instance().resetBits(MXT_PORT(led), MXT_PINS(led));
}

}

