#include "hw/Motor.h"
#include "hw/Actuator.h"
#include "hw/HWAccess.h"

namespace hw
{

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
	uint8_t a, n;

	n = a = HWAccess::instance().in(HWAccessImpl::PORT_A);

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
		HWAccess::instance().out(HWAccessImpl::PORT_A, n);
}

void Motor::doControlSwitch(const void *vp)
{
	const motor_switch_packet *p = static_cast<const motor_switch_packet *>(vp);
	pid_t f = p->state;
	(HWAccess::instance().*(f ? &HWAccessImpl::resetBits : &HWAccessImpl::setBits))(HWAccessImpl::PORT_A, SWITCH);
}

}

