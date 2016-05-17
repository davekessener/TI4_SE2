#include <vector>

#include "run/Project.h"
#include "run/State.h"

namespace haw
{
using lib::Time;
using lib::Timer;
using lib::Speed;

void Project::run(void)
{
	std::vector<State_ptr> states;
	Timer fps;
	Time delay = Time::ms(1), delta(0);
	int idx = 0;

	states.push_back(State_ptr(new Initialization(*this)));
	states.push_back(State_ptr(new Ready));

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
				states[idx += ni]->enter();
			}

			if(hm_.running())
				hm_.tick();

			delta = fps.sync(delay);
		}
		catch(...)
		{
			states[idx].exit();
			MXT_TODO_ERROR; //TODO
			states[idx].enter();

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

	hmPos_ = speeds_[SPEED_SLOW].in(toHM);
	pukWidth_ = speeds_[SPEED_SLOW].in(puk);
}

}

