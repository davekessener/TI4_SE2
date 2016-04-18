#include <iostream>
#include <lib/TimeP.h>
#include <hw/HWAccess.h>
#include <hw/LED.h>
#include <hw/Actuator.h>

int main(int argc, char *argv[])
{
	lib::Time delay = lib::Time::s(1);

	std::cout << "starting now" << std::endl;

	delay.wait();
	hw::LEDs::instance().turnOn(hw::LED::RED);
	delay.wait();
	hw::LEDs::instance().turnOn(hw::LED::GREEN);
	delay.wait();
	hw::LEDs::instance().turnOff(hw::LED::GREEN);
	delay.wait();
	hw::LEDs::instance().turnOff(hw::LED::RED);

	lib::Time::s(3).wait();

	std::cout << "leaving now" << std::endl;

	return 0;
}
