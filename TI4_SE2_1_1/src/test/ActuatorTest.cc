#include "test/ActuatorTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/LED.h"
#include "hw/Motor.h"
#include "hw/Actuator.h"

namespace test
{
	void actuators(void)
	{
		lib::Time delay = lib::Time::s(1);

		hw::LEDs::instance().turnOn(hw::LED::RED);
		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(2.0));
		hw::LEDs::instance().turnOn(hw::LED::GREEN);
		hw::LEDs::instance().turnOn(hw::LED::START);
		hw::LEDs::instance().turnOn(hw::LED::RESET);
		hw::LEDs::instance().turnOn(hw::LED::Q1);
		hw::LEDs::instance().turnOn(hw::LED::Q2);
		delay.wait();

		hw::Motors::instance().controlSwitch(hw::Motor::State::OPEN);
		hw::LEDs::instance().turnOff(hw::LED::GREEN);
		delay.wait();

		hw::Motors::instance().controlSwitch(hw::Motor::State::CLOSE);
		hw::LEDs::instance().turnOff(hw::LED::RED);
		hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::FAST);
		lib::Time::s(3).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::LEFT, hw::Motor::Speed::SLOW);

		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(0.5));

		lib::Time::s(6).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::NONE, hw::Motor::Speed::STOP);

		hw::LEDs::instance().turnOff(hw::LED::YELLOW);
		hw::LEDs::instance().turnOff(hw::LED::START);
		hw::LEDs::instance().turnOff(hw::LED::RESET);
		hw::LEDs::instance().turnOff(hw::LED::Q1);
		hw::LEDs::instance().turnOff(hw::LED::Q2);

		delay.wait();
	}
}
