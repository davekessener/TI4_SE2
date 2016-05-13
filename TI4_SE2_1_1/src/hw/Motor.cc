#include "hw/Motor.h"
#include "hw/Actuator.h"
#include "hw/HWAccess.h"
#include "lib/Data.h"

namespace hw
{

using lib::Data;

namespace
{
	struct motor_belt_packet
	{
		uint8_t cmd;
		Motor::pid_t dir;
		Motor::pid_t speed;
	};

	struct motor_switch_packet
	{
		uint8_t cmd;
		Motor::pid_t state;
	};
}

Motor::Motor(void) : curDir_(Direction::NONE), con_(Actuators::instance().getChannel().connect())
{
}

void Motor::controlBelt(pid_t dir, pid_t speed)
{
	Lock guard(this);

	motor_belt_packet p;

	p.cmd = Actuator::MOTOR_BELT;
	p.dir = dir;
	p.speed = speed;

	con_.send(Data::get(p));
}

void Motor::controlSwitch(pid_t state)
{
	Lock guard(this);

	motor_switch_packet p;

	p.cmd = Actuator::MOTOR_SWITCH;
	p.state = state;

	con_.send(Data::get(p));
}

// # ---------------------------------------------------------------------------

void Motor::doControlBelt(const void *vp)
{
	const motor_belt_packet *p = static_cast<const motor_belt_packet *>(vp);
	pid_t d = p->dir, s = p->speed;
	uint8_t n;

	n = HWAccess::instance().in(HWAccessImpl::PORT_A);

	if(s == Speed::STOP)
	{
		n = MXT_RESETBITS(n, Direction::LEFT | Direction::RIGHT | Speed::SLOW);
	}
	else
	{
		if(d != Direction::NONE)
		{
			n = MXT_RESETBITS(n, Direction::LEFT | Direction::RIGHT);
			n = MXT_SETBITS(n, d);
		}

		n = (s == Speed::FAST) ? MXT_RESETBITS(n, Speed::SLOW) : MXT_SETBITS(n, Speed::SLOW);
	}

	HWAccess::instance().out(HWAccessImpl::PORT_A, n);
}

void Motor::doControlSwitch(const void *vp)
{
	const motor_switch_packet *p = static_cast<const motor_switch_packet *>(vp);
	pid_t f = p->state;
	(HWAccess::instance().*(f ? &HWAccessImpl::resetBits : &HWAccessImpl::setBits))(HWAccessImpl::PORT_A, SWITCH);
}

}

