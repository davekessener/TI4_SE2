#include "test/HWTest.h"

#include "hw/Actuator.h"
#include "hw/Sensor.h"
#include "lib/Observer.hpp"
#include "lib/TimeP.h"

namespace test {

using hw::Actuators;
using hw::Sensors;
using hw::Sensor;
using hw::Belt;
using hw::Switch;
using hw::LED;
using lib::Frequency;

bool running = false;

class Test : public lib::Observer<Sensor::sensor_t>
{
	typedef Sensor::sensor_t sensor_t;

	public:
		virtual void update(const sensor_t&);
};

void Test::update(const sensor_t& sensor)
{
	std::cout << "sensor " << sensor << " changed to " << Sensors::instance().get(sensor) << "." << std::endl;

	switch(sensor)
	{
		case 0:
			Actuators::instance().getLeds().turnOn(LED::RED);
			Actuators::instance().getLeds().turnOn(LED::YELLOW);
			Actuators::instance().getLeds().turnOn(LED::GREEN);
			break;
		case 1:
			Actuators::instance().getLeds().blink(LED::RED, Frequency::Hz(0.5));
			Actuators::instance().getLeds().blink(LED::YELLOW, Frequency::Hz(1));
			Actuators::instance().getLeds().blink(LED::GREEN, Frequency::Hz(2));
			break;
		case 2:
			Actuators::instance().getLeds().turnOff(LED::RED);
			Actuators::instance().getLeds().turnOff(LED::YELLOW);
			Actuators::instance().getLeds().turnOff(LED::GREEN);
			break;
		case 3:
			Actuators::instance().getBelt().move(Belt::RIGHT, Belt::FAST);
			break;
		case 4:
			Actuators::instance().getBelt().move(Belt::LEFT, Belt::FAST);
			break;
		case 5:
			Actuators::instance().getBelt().changeSpeed(Belt::SLOW);
			break;
		case 6:
			Actuators::instance().getBelt().changeSpeed(Belt::SLOW);
			break;
		case 7:
			Actuators::instance().getBelt().pause();
			break;
		case 8:
			Actuators::instance().getBelt().unpause();
			break;
		case 9:
			Actuators::instance().getBelt().stop();
			break;
		case 10:
			if(Sensors::instance().get(sensor))
			{
				Actuators::instance().getSwitch().open();
			}
			else
			{
				Actuators::instance().getSwitch().close();
			}
			break;
		case 11:
			running = false;
			break;
	}
}

void hw(void)
{
	Test test;
	running = true;

	Sensors::instance().attach(test);

	while(running) Time::ms(100).wait();

	Actuators::instance().getLeds().turnOff(LED::RED);
	Actuators::instance().getLeds().turnOff(LED::YELLOW);
	Actuators::instance().getLeds().turnOff(LED::GREEN);
	Actuators::instance().getBelt().stop();
	Actuators::instance().getSwitch().open();
}

}

