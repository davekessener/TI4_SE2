#include "test/ActuatorTest.h"
#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/LED.h"
#include "hw/Motor.h"
#include "hw/Actuator.h"
#include "hw/Sensor.h"
#include "lib/log/LogManager.h"

namespace test
{
	void actuators(void)
	{
		using lib::Time;
		using lib::log::Logger_ptr;
		using lib::log::LogManager;
		using hw::LEDs;
		using hw::LED;
		using hw::Motor;
		using hw::Motors;

		hw::Sensors::instance();

		Time delay = Time::ms(500);
		Logger_ptr log = LogManager::instance().rootLog();

		log->MXT_LOG("turn on GREEN");
        LEDs::instance().turnOn(LED::GREEN);
        delay.wait();

		log->MXT_LOG("turn off GREEN and turn on YELLOW");
        LEDs::instance().turnOff(LED::GREEN);
        LEDs::instance().turnOn(LED::YELLOW);
        delay.wait();

		log->MXT_LOG("turn off YELLOW and turn on RED");
        LEDs::instance().turnOff(LED::YELLOW);
        LEDs::instance().turnOn(LED::RED);
        delay.wait();

		log->MXT_LOG("turn off RED and blink GREEN @2Hz");
        LEDs::instance().turnOff(LED::RED);
        LEDs::instance().blink(LED::GREEN, lib::Frequency::Hz(2.0));
        delay.wait();

		log->MXT_LOG("blink GREEN @0.5Hz");
        LEDs::instance().blink(LED::GREEN, lib::Frequency::Hz(0.5));
        delay.wait();

		log->MXT_LOG("blink YELLO @2Hz");
        LEDs::instance().blink(LED::YELLOW, lib::Frequency::Hz(2.0));
        delay.wait();

		log->MXT_LOG("blink YELLOW @0.5Hz");
        LEDs::instance().blink(LED::YELLOW, lib::Frequency::Hz(0.5));
        delay.wait();

		log->MXT_LOG("blink RED @2Hz");
        LEDs::instance().blink(LED::RED, lib::Frequency::Hz(2.0));
        delay.wait();

		log->MXT_LOG("blink RED @0.5Hz");
        LEDs::instance().blink(LED::RED, lib::Frequency::Hz(0.5));
        delay.wait();

		log->MXT_LOG("turn on BELT RIGHT FAST");
        Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::FAST);
        delay.wait();

		log->MXT_LOG("turn on BELT RIGHT SLOW");
        Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::SLOW);
        delay.wait();

		log->MXT_LOG("turn on BELT RIGHT FAST");
        Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::FAST);
        delay.wait();

		log->MXT_LOG("turn on BELT RIGHT SLOW");
        Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::SLOW);
        delay.wait();

		log->MXT_LOG("turn off BELT");
        Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
        delay.wait();

		log->MXT_LOG("open SWITCH");
        Motors::instance().controlSwitch(Motor::State::OPEN);
        delay.wait();

		log->MXT_LOG("close SWITCH and turn on START");
        Motors::instance().controlSwitch(Motor::State::CLOSE);
		LEDs::instance().turnOn(LED::START);
        delay.wait();

		log->MXT_LOG("turn on RESET");
		LEDs::instance().turnOn(LED::RESET);
        delay.wait();

		log->MXT_LOG("turn on Q1");
		LEDs::instance().turnOn(LED::Q1);
        delay.wait();

		log->MXT_LOG("turn on Q2");
		LEDs::instance().turnOn(LED::Q2);
		delay.wait();

		log->MXT_LOG("turn ALL off");
		LEDs::instance().turnOff(LED::RED);
		LEDs::instance().turnOff(LED::YELLOW);
		LEDs::instance().turnOff(LED::GREEN);
		LEDs::instance().turnOff(LED::START);
		LEDs::instance().turnOff(LED::RESET);
		LEDs::instance().turnOff(LED::Q1);
		LEDs::instance().turnOff(LED::Q2);
		Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
		Motors::instance().controlSwitch(Motor::State::CLOSE);

		log->MXT_LOG("DONE testing actuators. ---------------------------------");

		hw::Sensors::instance().shutdown();
	}
}
