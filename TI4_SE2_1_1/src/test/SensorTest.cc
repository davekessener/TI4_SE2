#include <stdio.h>

#include "hw/Sensor.h"
#include "lib/Timer.h"
#include "lib/TimeP.h"
#include "lib/log/LogManager.h"
#include "hw/HWAccess.h"
#include "hw/Motor.h"
#include "hw/LED.h"

#include "test/SensorTest.h"
#include "lib/qnx/ISR.h"

namespace test {

using hw::Sensors;
using hw::Sensor;
using lib::Timer;
using lib::Time;
using hw::LEDs;
using hw::LED;
using hw::Motors;
using hw::Motor;
using lib::log::LogManager;
using lib::log::Logger_ptr;

void runHM(Sensor &sensors)
{
	Timer fps;
	Time delay = Time::ms(1);
	std::vector<uint16_t> values;

	while(sensors.inHM())
	{
		values.push_back(sensors.getHeight());

		fps.sync(delay);
	}

	if(values.empty())
	{
		printf("no values measured\n");
		return;
	}

#define LINES 6
#define COLS 10

	int n = values.size() / LINES, p = 0, s = values.size();
	uint16_t min = -1, max = 0, avg;
	std::vector<uint16_t> disp;

	for(int i = 0 ; i < LINES ; ++i)
	{
		uint32_t t = 0;
		for(int j = p ; j < p + n && j < s ; ++j)
		{
			t += values[j];
		}

		p += n;
		t /= n;

		if(t < min) min = t;
		if(t > max) max = t;

		disp.push_back(t);
	}

	avg = (min + max) / 2;

	for(int i = 0 ; i < LINES ; ++i)
	{
		disp[i] = disp[i] <= avg ? 0 : COLS;
	}

	for(int i = 0 ; i < LINES ; ++i)
	{
		printf("%*s\n", disp[i], "#");
	}

	printf("Max is 0x%04x\n", max);
}

void sensors(void)
{
	Logger_ptr log = LogManager::instance().getLog(MXT_LOG_ISR);
	log->addParent(LogManager::instance().rootLog());
	log->MXT_LOG("Starting sensors loop");

	Sensor &sensors = Sensors::instance();
	Timer fps;
	Time delay = Time::ms(1);

	LEDs::instance().turnOn(LED::START);
	LEDs::instance().turnOn(LED::YELLOW);
	log->MXT_LOG("Please press START for testrun"); while(!sensors.start()) fps.sync(delay);
	LEDs::instance().turnOff(LED::START);

	LEDs::instance().blink(LED::YELLOW, lib::Frequency::Hz(0.5));
	while(!sensors.entering()) fps.sync(delay);
	LEDs::instance().turnOff(LED::YELLOW);
	LEDs::instance().turnOn(LED::GREEN);
	log->MXT_LOG("-> going to hm");
	Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::FAST);
	while(!sensors.inHM()) fps.sync(delay);
	log->MXT_LOG("-> starting hw");
	Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::SLOW);
	runHM(sensors);
	Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::FAST);
	while(!sensors.inSwitch()) fps.sync(delay);
	Motors::instance().controlSwitch(Motor::State::OPEN);
	bool metal = sensors.isMetal();
	Time::ms(150).wait();
	if(!metal)
	{
		while(!sensors.leaving()) fps.sync(delay);
		Motors::instance().controlSwitch(Motor::State::CLOSE);
	}
	else
	{
		Motors::instance().controlSwitch(Motor::State::CLOSE);
		LEDs::instance().turnOff(LED::GREEN);
		LEDs::instance().blink(LED::RED, lib::Frequency::Hz(1.0));
		log->MXT_LOG("metal detected!");
		log->MXT_LOG("press RESET");
		LEDs::instance().turnOn(LED::RESET);
		while(!sensors.reset()) fps.sync(delay);
		LEDs::instance().turnOff(LED::RESET);
	}
	Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
	LEDs::instance().turnOff(LED::GREEN);
	LEDs::instance().turnOff(LED::YELLOW);
	LEDs::instance().turnOff(LED::RED);

	Sensors::instance().shutdown();
}

}
