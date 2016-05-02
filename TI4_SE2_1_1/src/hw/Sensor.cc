#include "hw/Sensor.h"

#include "lib/TimeP.h"

namespace hw
{

Sensor::Sensor(void)
{
	thread_.reset(new lib::Thread(lib::wrapInFtor(this, &Sensor::thread)));

	while(!ch_.isOpen()) lib::Time::ms(2).wait();
}

Sensor::~Sensor(void)
{
	if(static_cast<bool>(thread_.get()) && thread_->joinable())
		thread_->join();
}

void Sensor::thread(void)
{
	lib::qnx::Receiver recv = ch_.open();
	running_ = true;

	while(running_)
	{
	}
}

}

