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
		lib::Time delay = lib::Time::s(3);

        hw::LEDs::instance().turnOn(hw::LED::GREEN);
        delay.wait();
        hw::LEDs::instance().turnOff(hw::LED::GREEN);
        hw::LEDs::instance().turnOn(hw::LED::YELLOW);
        delay.wait();
        hw::LEDs::instance().turnOff(hw::LED::YELLOW);
        hw::LEDs::instance().turnOn(hw::LED::RED);
        delay.wait();
        hw::LEDs::instance().turnOff(hw::LED::RED);
        hw::LEDs::instance().blink(hw::LED::GREEN, lib::Frequency::Hz(2.0));
        delay.wait();
        hw::LEDs::instance().blink(hw::LED::GREEN, lib::Frequency::Hz(0.5));
        delay.wait();
        hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(2.0));
        delay.wait();
        hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(0.5));
        delay.wait();
        hw::LEDs::instance().blink(hw::LED::RED, lib::Frequency::Hz(2.0));
        delay.wait();
        hw::LEDs::instance().blink(hw::LED::RED, lib::Frequency::Hz(0.5));
        delay.wait();
        hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::FAST);
        delay.wait();
        hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::SLOW);
        delay.wait();
        hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::FAST);
        delay.wait();
        hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::SLOW);
        delay.wait();
        hw::Motors::instance().controlBelt(hw::Motor::Direction::NONE, hw::Motor::Speed::STOP);
        delay.wait();
        hw::Motors::instance().controlSwitch(hw::Motor::State::OPEN);
        delay.wait();
        hw::Motors::instance().controlSwitch(hw::Motor::State::CLOSE);
		hw::LEDs::instance().turnOn(hw::LED::START);
        delay.wait();
		hw::LEDs::instance().turnOn(hw::LED::RESET);
        delay.wait();
		hw::LEDs::instance().turnOn(hw::LED::Q1);
        delay.wait();
		hw::LEDs::instance().turnOn(hw::LED::Q2);
		delay.wait();
	}
}
