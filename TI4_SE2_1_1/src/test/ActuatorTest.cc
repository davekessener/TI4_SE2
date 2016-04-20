#include "test/ActuatorTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/Motor.h"
#include "hw/Actuator.h"

namespace test
{
	/**
	* \class ActuatorTest
	*
	* \brief Testet die Aktoren
	*
	* Führt eine Testreihe der Aktoren durch.
	* Zunächst wird das Laufband mit allen Operationen gestartet.
	* Danach wird die Weiche geöffnet und geschlossen.
	*
	* \author Malte Schuler
	*
	* \version $Revision: 1.0 $
	*
	* \date $Date: 2016/04/20 13:01:00 $
	*
	*/
	void actuators(void)
	{
		lib::Time delay = lib::Time::s(1);

		std::cout << "starting now" << std::endl;

		delay.wait();
		hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::FAST);

		lib::Time::s(3).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::LEFT, hw::Motor::Speed::FAST);

		lib::Time::s(3).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::LEFT, hw::Motor::Speed::SLOW);

		lib::Time::s(4).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::SLOW);

		lib::Time::s(4).wait();

		std::cout << "stop motor" << std::endl;

		std::cout << "open gate" << std::endl;

		hw::Motors::instance().controlBelt(hw::Motor::Direction::NONE, hw::Motor::Speed::STOP);

		hw::Motors::instance().controlSwitch(hw::Motor::State::OPEN);

		delay.wait();

		hw::Motors::instance().controlSwitch(hw::Motor::State::CLOSE);

		delay.wait();

		hw::Motors::instance().controlSwitch(hw::Motor::State::OPEN);

		delay.wait();

		hw::Motors::instance().controlSwitch(hw::Motor::State::CLOSE);

		std::cout << "close gate" << std::endl;

		delay.wait();

		std::cout << "leaving now" << std::endl;
	}
}
