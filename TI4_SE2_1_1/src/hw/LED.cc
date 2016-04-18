#include "hw/LED.h"
#include "hw/Actuator.h"

namespace hw
{

LED::LED(void) : con_(Actuators::instance().getChannel().connect())
{
}

void LED::turnOn(led_t led)
{
	Lock guard(this);
}

void LED::turnOff(led_t led)
{
	Lock guard(this);
}

void LED::blink(led_t led, const Time& f)
{
	Lock guard(this);
}

// # ---------------------------------------------------------------------------

void LED::doTurnOn(led_t led)
{
	HWAccess::instance().setBits(MXT_PORT(led), MXT_PINS(led));
}

void LED::doTurnOff(led_t led)
{
	HWAccess::instance().resetBits(MXT_PORT(led), MXT_PINS(led));
}

}

