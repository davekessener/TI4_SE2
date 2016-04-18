#include "hw/Actuator.h"

namespace lib
{
	Actuator::Actuator(void)
	: thread_(lib::FtorWrapper(this, &Actuator::thread)), running_(false)
	{
	}

	void Actuator::thread(void)
	{
		lib::qnx::Receiver recv = ch_.open();
		running_ = true;

		while(running_)
		{
			lib::Packet_ptr p = recv.receive();

			// TODO
		}
	}
}

