#include "test/ActuatorTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/Motor.h"
#include "hw/Actuator.h"

namespace test
{
	void actuators(void)
	{
		lib::Time delay = lib::Time::s(1);

		std::cout << "starting now" << std::endl;

		delay.wait();
		hw::Motors::instance().controlBelt(hw::Motor::Direction::RIGHT, hw::Motor::Speed::FAST);

		lib::Time::s(3).wait();

		hw::Motors::instance().controlBelt(hw::Motor::Direction::LEFT, hw::Motor::Speed::SLOW);

		std::cout << "stop motor" << std::endl;

		lib::Time::s(6).wait();

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
