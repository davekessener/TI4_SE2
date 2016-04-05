#include "LightsController.h"

namespace SE
{

LightsController& LightsController::instance(void)
{
	static LightsController inst;

	return inst;
}

void LightsController::turnOn(std::uint16_t bitmask) const
{
	buf_.enqueue(std::make_pair(true, bitmask));
}

void LightsController::turnOff(std::uint16_t bitmask) const
{
	buf_.enqueue(std::make_pair(false, bitmask));
}

void LightsController::light_controller_thread(void)
{
	LightsController &c(LightsController::instance());
	Lights &lights(Lights::instance());

	c.running_ = true;

	while(true)
	{
		value_type v = buf_.dequeue();

		if(!running()) break;

		(lights.*(v.first ? &Lights::turnOn : &Lights::turnOff))(v.second);
	}
}

}

