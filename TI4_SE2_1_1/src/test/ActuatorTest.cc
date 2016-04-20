#include "test/ActuatorTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/LED.h"

namespace test
{
<<<<<<< HEAD:TI4_SE2_1_1/src/test/ActuatorTest.cc
	void actuators(void)
	{
		lib::Time delay = lib::Time::s(1);

=======
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
>>>>>>> 902cdcc8d737b93fc0bd579042caa36a41c3508a:TI4_SE2_1_1/src/test/LEDTest.cc
		hw::LEDs::instance().turnOn(hw::LED::RED);
		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(2.0));
		hw::LEDs::instance().turnOn(hw::LED::GREEN);
		hw::LEDs::instance().turnOn(hw::LED::START);
		hw::LEDs::instance().turnOn(hw::LED::RESET);
		hw::LEDs::instance().turnOn(hw::LED::Q1);
		hw::LEDs::instance().turnOn(hw::LED::Q2);
		delay.wait();
<<<<<<< HEAD:TI4_SE2_1_1/src/test/ActuatorTest.cc

		hw::Motors::instance().controlSwitch(hw::Motor::State::OPEN);
		hw::LEDs::instance().turnOff(hw::LED::GREEN);
		delay.wait();

		hw::Motors::instance().controlSwitch(hw::Motor::State::CLOSE);
		hw::LEDs::instance().turnOff(hw::LED::RED);
		hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::FAST);
		lib::Time::s(3).wait();
=======
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
>>>>>>> 902cdcc8d737b93fc0bd579042caa36a41c3508a:TI4_SE2_1_1/src/test/LEDTest.cc

		lib::Time::s(1).wait();

		hw::LEDs::instance().blink(hw::LED::YELLOW, lib::Frequency::Hz(0.5));

		lib::Time::s(6).wait();

<<<<<<< HEAD:TI4_SE2_1_1/src/test/ActuatorTest.cc
		hw::Motors::instance().controlBelt(hw::Motor::Direction::NONE, hw::Motor::Speed::STOP);
=======
		std::cout << "done blinking" << std::endl;
>>>>>>> 902cdcc8d737b93fc0bd579042caa36a41c3508a:TI4_SE2_1_1/src/test/LEDTest.cc

		hw::LEDs::instance().turnOff(hw::LED::YELLOW);
		hw::LEDs::instance().turnOff(hw::LED::START);
		hw::LEDs::instance().turnOff(hw::LED::RESET);
		hw::LEDs::instance().turnOff(hw::LED::Q1);
		hw::LEDs::instance().turnOff(hw::LED::Q2);

		delay.wait();
	}
}
