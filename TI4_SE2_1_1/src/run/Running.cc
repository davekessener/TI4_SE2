#include "run/Running.h"

namespace haw
{

Running(Project& p) 
	: project_(p), speed_(Project::SPEED_FAST), toSwitchClose_(0), nextPuk_(PUK_FLAT),
	  stopping_(false), pausing_(false)
{
	uint32_t w = project_.getPukWidth();
	scKeep_ = 3 * w / 2;
	scKick_ = w / 2;
}

void Running::enter(void)
{
	Motors::instance().controlBelt(Motor::Direction::RIGHT, getSpeed(speed_));
}

void Running::exit(void)
{
	Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
}

void Running::update(lib::Time t)
{
	uint32_t d = project_.getSpeed(speed_).in(t);

	for(iter_t i1 = puks_.begin(), i2 = puks_.end() ; i1 != i2 ; ++i1)
	{
		i1->update(d);
	}

	if(!switchClosed())
	{
		if((toSwitchClose_ -= (int32_t)d) <= 0)
		{
			Motors::instance().controlSwitch(Motor::State::CLOSE);
		}
	}
}

void Running::process(const Event& event)
{
	if(event.id() == Event::ID::SENSOR)
	{
		const SensorEvent &e = *reinterpret_cast<const SensorEvent *>(&event);

		switch(e.sensor())
		{
			case SensorEvent::Sensors::ENTERING:
				if(!e.value())
				{
					puks_.push_back(Puk(project_.getPukWidth()));
				}
				else if(stopping_)
				{
					MXT_TODO_ERROR; //TODO
				}
				break;
			case SensorEvent::Sensors::LEAVING:
				if(e.value())
				{
					enterLeaving();
				}
				else
				{
					exitLeaving();
				}
				break;
			case SensorEvent::Sensors::IN_HM:
				if(e.value())
					enterHM();
				else
					exitHM();
				break;
			case SensorEvent::Sensors::IN_SWITCH:
				if(e.value())
					handleSwitch();
				break;
			case SensorEvent::Sensors::STOP:
				if(e.value())
				{
					stopping_ = true;
				}
				break;
			case SensorEvent::Sensors::ESTOP:
				MXT_TODO_ERROR; //TODO
				break;
		}
	}
}

void Running::execute(void)
{
	iter_t ipuk = anyPukIn(project_.hmPosition());

	if(ipuk != puks_.end() && !Sensors::instance().inHM())
		MXT_TODO_ERROR; //TODO

	ipuk = anyPukIn(project_.endPosition());

	if(ipuk != puks_.end() && !Sensors::instance().leaving())
		MXT_TODO_ERROR; //TODO
	
	if(switchClosed())
	{
		if(stopping_)
		{
			setNext(State::PREVIOUS);
		}
		else if(pausing_)
		{
			setNext(State::NEXT);
		}
	}
}

// # ---------------------------------------------------------------------------

void Running::enterHM(void)
{
	iter_t ipuk = anyPukIn(project_.hmPosition());

	if(ipuk == puks_.end())
		MXT_TODO_ERROR; //TODO
	else
	{
		hmPuk_ = &*ipuk;
		project_.startHM();
		Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::SLOW);
	}
}

void Running::exitHM(void)
{
	hmPuk_->setType(project_.stopHM());
	Motors::instance().controlBelt(Motor::Direction::RIGHT, getSpeed(speed_));
}

void Running::enterLeaving(void)
{
	iter_t ipuk = anyPukIn(project_.endPosition());

	if(ipuk == puks_.end())
		MXT_TODO_ERROR; //TODO

	puks_.erase(ipuk);
}

void Running::exitLeaving(void)
{
}

void Running::handleSwitch(void)
{
	if(!hmPuk_)
		MXT_TODO_ERROR; //TODO

	bool keep = keepPuk(*hmPuk_);

	Motors::instance().controlSwitch(Motor::State::OPEN);

	toSwitchClose_ = keep ? scKeep_ : scKick_;

	if(!keep)
	{
		puks_.erase(std::find(puks_.begin(), puks_.end(), *hmPuk_));
	}

	hmPuk_ = NULL;
}

bool Running::keepPuk(Puk& puk)
{
	if(puk.upsideDown() || puk.type() == Puk::Type::UNKNOWN)
		return false;

	bool same = nextPuk_ == toType(puk);

	if(same)
	{
		nextPuk_ = (nextPuk_ + 1) % CPUKS;
	}

	return same || Sensors::instance().rampFull() || puk.metal();
}

Running::iter_t Running::anyPukIn(uint32_t p)
{
	for(iter_t i1 = puks_.begin(), i2 = puks_.end() ; i1 != i2 ; ++i1)
	{
		if(i1->isIn(p))
			return i1;
	}

	return puks_.end();
}

}

