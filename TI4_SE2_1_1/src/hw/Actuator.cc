#include "hw/Actuator.h"
#include "lib/TimeP.h"
#include "lib/Data.h"

namespace hw
{
	using lib::Data;
	using lib::Data_ptr;

	Actuator::Actuator(void)
	: running_(false)
	{
		cmds_[LED_ACTIVATE] = &Actuator::led_activate;
		cmds_[MOTOR_BELT] = &Actuator::motor_belt;
		cmds_[MOTOR_SWITCH] = &Actuator::motor_switch;

		thread_.reset(new Thread(lib::wrapInFtor(this, &Actuator::thread)));

		while(!ch_.isOpen()) lib::Time::ms(2).wait();
	}

	void Actuator::thread(void)
	{
		lib::qnx::Receiver recv = ch_.open();
		HWAccess::instance().initThread();

		running_ = true;

		while(running_)
		{
			Data_ptr p = recv.receive();

			if(!static_cast<bool>(p)) continue;

			uint8_t cmd = *reinterpret_cast<const uint8_t *>(p->data());

			(this->*cmds_[cmd])(p->data());
		}
	}
}

