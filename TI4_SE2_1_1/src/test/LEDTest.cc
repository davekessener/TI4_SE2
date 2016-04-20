#include "test/LEDTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/LED.h"

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
		hw::LEDs::instance().turnOff(hw::LED::GREEN);
		delay.wait();
		hw::LEDs::instance().turnOff(hw::LED::RED);

		lib::Time::s(1).wait();

		std::cout << "commence blinking" << std::endl;

		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(0.5));

		lib::Time::s(6).wait();

		std::cout << "done blinking" << std::endl;

		hw::LEDs::instance().turnOff(hw::LED::YELLOW);

		delay.wait();

		std::cout << "leaving now" << std::endl;
	}
}
