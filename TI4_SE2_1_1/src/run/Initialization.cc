#include "run/Initialization.h"

#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/Sensor.h"
#include "hw/Actuator.h"
#include "hw/LED.h"
#include "hw/Motor.h"

namespace haw
{

using hw::LEDs;
using hw::LED;
using hw::Sensors;

// # ===========================================================================

void Calibrator::Measurer::tick(void)
{
}

void Calibrator::Measurer::start(const SensorEvent& e)
{
}

void Calibrator::Measurer::toHM(const SensorEvent& e)
{
}

void Calibrator::Measurer::inHM(const SensorEvent& e)
{
}

void Calibrator::Measurer::back(const SensorEvent& e)
{
}

// # ===========================================================================

void Calibrator::measureFlat(const SensorEvent& e)
{
}

void Calibrator::measureLarge(const SensorEvent& e)
{
}

void Calibrator::start(const SensorEvent& e)
{
}

void Calibrator::toHM(const SensorEvent& e)
{
}

void Calibrator::toSwitch(const SensorEvent& e)
{
}

void Calibrator::toEnd(const SensorEvent& e)
{
}

// # ===========================================================================

Initialization::Initialization(void)
: next_(State::THIS)
{
}

void Initialization::enter(void)
{
	hw::HWAccess::instance();
	hw::Sensors::instance();
	hw::Actuators::instance();
	hw::LEDs::instance();
	hw::Motors::instance();

	LEDs::instance().turnOn(LED::YELLOW);

	std::cout << "Starting calibrations ..." << std::endl;
}

void Initialization::exit(void)
{
	LEDs::instance().turnOff(LED::YELLOW);
}

void Initialization::process(const Event& e)
{
	if(e.id() == Event::ID::SENSOR)
	{
		const SensorEvent &se = *reinterpret_cast<const SensorEvent *>(&e);

		if(se.sensor() == SensorEvent::Sensors::START && se.value())
		{
			setNext(State::NEXT);
		}
	}
}

// # ===========================================================================

}

