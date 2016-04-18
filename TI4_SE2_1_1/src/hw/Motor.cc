#include "hw/Motor.h"

namespace hw
{

Motor::Motor(void) : curDir_(Direction::NONE), con_(Actuators::instance().getChannel().connect())
{
}

void Motor::controlBelt(pid_t dir, pid_t speed)
{
	Lock guard(this);
}

void Motor::controlSwitch(pid_t state)
{
	Lock guard(this);
}

// # ---------------------------------------------------------------------------

void Motor::doControlBelt(pid_t d, pid_t s)
{
	uint8_t a, n;

	n = a = HWAccess::instance().in(HWAccess::PORT_A);

	if(curDir_ != Direction::NONE && (curDir_ != d || s == Speed::STOP))
	{
		n = MXT_RESETBITS(n, curDir_);
		curDir_ = Direction::NONE;
	}

	if(d != Direction::NONE && d != curDir_ && s != Speed::STOP)
	{
		n = MXT_SETBITS(n, d|s);
		curDir_ = d;
	}

	if(n != a)
		HWAccess::instance().out(HWAccess::PORT_A, n);
}

void Motor::doControlSwitch(pid_t f)
{
	(HWAccess::instance().*(f ? &HWAccess::resetBits : &HWAccess::setBits))(HWAccess::PORT_A, SWITCH);
}

}

