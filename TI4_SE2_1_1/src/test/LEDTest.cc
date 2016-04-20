#include "test/LEDTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/LED.h"

namespace test
{
	/**
	* \class LEDTest
	*
	* \brief Testet die LEDs
	*
	* Führt eine Testreihe der LEDs durch.
	* Testet das separate Anschalten der LEDs der Ampel.
	* Sowie das blinken der Ampel.
	* Außerdem die LEDs an Port C. (An der Vorderseite)
	*
	* \author Daniel Kessener, Malte Schuler
	*
	* \version $Revision: 1.3 $
	*
	* \date $Date: 2016/04/20 13:10:00 $
	*
	*/
	void leds(void)
	{
		lib::Time delay = lib::Time::s(1);

		std::cout << "starting now" << std::endl;

		//Ampel
		delay.wait();
		hw::LEDs::instance().turnOn(hw::LED::RED);
		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(2.0));
		hw::LEDs::instance().turnOn(hw::LED::GREEN);
		delay.wait();
		hw::LEDs::instance().turnOff(hw::LED::GREEN);
		delay.wait();
		hw::LEDs::instance().turnOff(hw::LED::RED);

		//Front LEDs
		hw::LEDs::instance().turnOn(hw::LED::START);
		delay.wait();
		hw::LEDs::instance().turnOff(hw::LED::START);
		hw::LEDs::instance().turnOn(hw::LED::RESET);
		delay.wait();
		hw::LEDs::instance().turnOff(hw::LED::RESET);
		hw::LEDs::instance().turnOn(hw::LED::Q1);
		delay.wait();
		hw::LEDs::instance().turnOff(hw::LED::Q1);
		hw::LEDs::instance().turnOn(hw::LED::Q2);
		delay.wait();
		hw::LEDs::instance().turnOff(hw::LED::Q2);

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
