#include <vector>
#include <iostream>

#include "run/Initialization.h"
#include "run/Ready.h"
#include "run/Running.h"
#include "run/Project.h"
#include "run/State.h"
#include "lib/Timer.h"
#include "hw/LED.h"
#include "hw/Motor.h"

namespace haw
{
using lib::Time;
using lib::Timer;
using lib::Speed;
using hw::LEDs;
using hw::LED;
using hw::Motors;
using hw::Motor;

Project::Project(void) : config_("/tmp/config.dat")
{
}

Project::~Project(void)
{
	LEDs::instance().turnOff(LED::GREEN);
	LEDs::instance().turnOff(LED::YELLOW);
	LEDs::instance().turnOff(LED::RED);
	Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
	Motors::instance().controlSwitch(Motor::State::CLOSE);
}

void Project::calibrateFromConfig(void)
{
	calibrateHM(config_.getMin(), config_.getMax());
	calibrateDistances(config_.getSlow(), config_.getFast(), config_.getHM(), config_.getPuk());
}

void Project::saveConfig(void)
{
	config_.save();
}

void Project::calibrateHM(uint16_t min, uint16_t max)
{
	hm_.calibrate(min, max);
	config_.setHM(min, max);
}

void Project::run(void)
{
	std::vector<State_ptr> states;
	Timer fps;
	Time delay = Time::ms(1), delta(0);
	int idx = 0;

	states.push_back(State_ptr(new Initialization(*this)));
	states.push_back(State_ptr(new Ready));
	states.push_back(State_ptr(new Running(*this)));

	states[0]->enter();

	while(true)
	{
		try
		{
			State_ptr s = states[idx];

			s->update(delta);

			processEvents(s);

			s->execute();

			State::update_t next = s->getNext();
			if(next != State::THIS)
			{
				s->exit();
				states[idx += next]->enter();
			}

			if(hm_.running())
				hm_.tick();

			delta = fps.sync(delay);
		}
		catch(const std::string& s)
		{
			std::cout << "caught string " << s << std::endl;
			return;
		}
		catch(const std::runtime_error& e)
		{
			std::cout << "caught runtime_error " << e.what() << std::endl;
			return;
		}
		catch(...)
		{
			states[idx]->exit();
			MXT_TODO_ERROR; //TODO
			states[idx]->enter();

			fps.reset();
		}
	}
}

void Project::processEvents(State_ptr s)
{
	hw::Sensor &sensor = hw::Sensors::instance();

	if(sensor.enteringChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::ENTERING, sensor.entering()));
	}

	if(sensor.leavingChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::LEAVING, sensor.leaving()));
	}

	if(sensor.startChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::START, sensor.start()));
	}

	if(sensor.stopChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::STOP, sensor.stop()));
	}

	if(sensor.resetChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::RESET, sensor.reset()));
	}

	if(sensor.estopChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::ESTOP, sensor.estop()));
	}

	if(sensor.inHMChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::IN_HM, sensor.inHM()));
	}

	if(sensor.inSwitchChanged())
	{
		s->process(SensorEvent(SensorEvent::Sensors::IN_SWITCH, sensor.inSwitch()));
	}
}

void Project::calibrateDistances(Time slow, Time fast, Time toHM, Time puk)
{
	speeds_[SPEED_STOP] = Speed(Time(0));
	speeds_[SPEED_SLOW] = Speed(slow);
	speeds_[SPEED_FAST] = Speed(fast);

	pukWidth_ = speeds_[SPEED_SLOW].in(puk);
	hmPos_ = speeds_[SPEED_SLOW].in(toHM) + 3 * pukWidth_ / 4;
	endPos_ = lib::Speed::reference - pukWidth_;

	config_.setTimes(slow, fast, toHM, puk);
}

}

