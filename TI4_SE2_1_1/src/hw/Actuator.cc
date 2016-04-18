#include "hw/Actuator.h"

namespace lib
{
	Actuator::Actuator(void)
	: thread_(lib::FtorWrapper(this, &Actuator::thread)), running_(false)
	{
		cmds_[LED_ACTIVATE] = &Actuator::led_activate;
		cmds_[MOTOR_BELT] = &Actuator::motor_belt;
		cmds_[MOTOR_SWITCH] = &Actuator::motor_switch;
	}

	void Actuator::thread(void)
	{
		lib::qnx::Receiver recv = ch_.open();
		running_ = true;

		while(running_)
		{
			lib::Packet_ptr p = recv.receive();
			uint8_t cmd = *reinterpret_cast<const uint8_t *>(p->data());

			cmds_[cmd](p->data());
		}
	}
}

