#include "run/Ready.h"

#include "hw/LED.h"

namespace haw
{

using hw::LEDs;
using hw::LED;

void Ready::enter(void)
{
	LEDs::instance().turnOn(LED::GREEN);
	setNext(State::THIS);
}

void Ready::exit(void)
{
//	LEDs::instance().turnOff(LED::GREEN);
}

void Ready::process(const Event& e)
{
	if(e.id() == Event::ID::SENSOR)
	{
		const SensorEvent &se = *reinterpret_cast<const SensorEvent *>(&e);

		if(se.sensor() == SensorEvent::Sensors::ENTERING && se.value())
		{
			setNext(State::NEXT);
		}
	}
}

}

