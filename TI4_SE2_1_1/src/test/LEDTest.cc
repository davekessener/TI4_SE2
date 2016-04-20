#include "test/LEDTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/LED.h"
#include "hw/Motor.h"
#include "hw/Actuator.h"

namespace test
{
	void leds(void)
	{
		lib::Time delay = lib::Time::s(1);

		std::cout << "starting now" << std::endl;

		delay.wait();
		hw::LEDs::instance().turnOn(hw::LED::RED);
		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(2.0));
		hw::LEDs::instance().turnOn(hw::LED::GREEN);
		delay.wait();
		hw::Motors::instance().controlSwitch(hw::Motor::State::OPEN);
		hw::LEDs::instance().turnOff(hw::LED::GREEN);
		delay.wait();
		hw::Motors::instance().controlSwitch(hw::Motor::State::CLOSE);
		hw::LEDs::instance().turnOff(hw::LED::RED);

		hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::FAST);

		lib::Time::s(3).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::LEFT, hw::Motor::Speed::SLOW);

		std::cout << "commence blinking" << std::endl;

		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(0.5));

		lib::Time::s(6).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::NONE, hw::Motor::Speed::STOP);

		std::cout << "done blinking" << std::endl;

		hw::LEDs::instance().turnOff(hw::LED::YELLOW);

		delay.wait();

		std::cout << "leaving now" << std::endl;
	}
}
