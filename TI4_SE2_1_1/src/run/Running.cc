#include <algorithm>

#include "run/Running.h"
#include "hw/Motor.h"
#include "hw/LED.h"
#include "hw/Sensor.h"

namespace haw
{

namespace
{
	inline int getSpeed(int s) { return s == Project::SPEED_SLOW ? 4 : (s == Project::SPEED_FAST ? 0 : 8); } // "undefined reference" error for some reason
	inline uint32_t toType(Puk& p) { return p.metal() ? 2 : (p.type() == 1 ? 0 : 1); }
//	inline int getSpeed(int s) { return s == Project::SPEED_SLOW ? hw::Motor::Speed::SLOW : (s == Project::SPEED_FAST ? hw::Motor::Speed::FAST : hw::Motor::Speed::STOP); }
//	inline uint32_t toType(Puk& p) { return p.metal() ? Running::PUK_METAL : (p.type() == Puk::Type::FLAT ? Running::PUK_FLAT : Running::PUK_LARGE); }
}

using hw::LEDs;
using hw::LED;
using hw::Motors;
using hw::Motor;
using hw::Sensors;

Running::Running(Project& p)
	: project_(p), speed_(Project::SPEED_FAST), toSwitchClose_(0), nextPuk_(PUK_FLAT), pid_(0),
	  stopping_(false), pausing_(false)
{
}

void Running::enter(void)
{
	std::cout << "entering running" << std::endl;
	setNext(State::THIS);
	uint32_t w = project_.getPukWidth();
	scKick_ = w / 3;
	scKeep_ = 3 * w;
	Motors::instance().controlBelt(Motor::Direction::RIGHT, getSpeed(speed_));
}

void Running::exit(void)
{
	std::cout << "leaving running" << std::endl;
	Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
}

void Running::update(lib::Time t)
{
	uint32_t d = project_.getSpeed(speed_).in(t);

	for(iter_t i1 = puks_.begin(), i2 = puks_.end() ; i1 != i2 ; ++i1)
	{
		(*i1)->update(d);
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
					puks_.push_back(Puk_ptr(new Puk(pid_++, project_.getPukWidth())));
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
	iter_t ipuk = anyPukIn(project_.hmPosition(), 4);

	if(ipuk != puks_.end() && !Sensors::instance().inHM())
		MXT_TODO_ERROR; //TODO

	ipuk = anyPukIn(project_.endPosition(), 4);

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

		if(!Sensors::instance().entering() && puks_.empty())
		{
			setNext(State::PREVIOUS);
		}
	}
}

// # ---------------------------------------------------------------------------

void Running::enterHM(void)
{
	iter_t ipuk = anyPukIn(project_.hmPosition(), 0);

	if(ipuk == puks_.end())
	{
		MXT_TODO_ERROR; //TODO
	}
	else
	{
		hmPuk_ = *ipuk;
		project_.startHM();
		speed_ = Project::SPEED_SLOW;
		Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::SLOW);
	}
}

void Running::exitHM(void)
{
	hmPuk_->setType(project_.stopHM());
	speed_ = Project::SPEED_FAST;
	Motors::instance().controlBelt(Motor::Direction::RIGHT, getSpeed(speed_));
}

void Running::enterLeaving(void)
{
	iter_t ipuk = anyPukIn(project_.endPosition(), 0);

	if(ipuk == puks_.end())
	{
		MXT_TODO_ERROR; //TODO
	}

	std::cout << "puk " << (*ipuk)->id() << " type " << (*ipuk)->type() << " and metal:" << (*ipuk)->metal() << std::endl;

	puks_.erase(ipuk);
}

void Running::exitLeaving(void)
{
}

void Running::handleSwitch(void)
{
	if(!hmPuk_)
		MXT_TODO_ERROR; //TODO

	if(Sensors::instance().isMetal())
		hmPuk_->setMetal();

	bool keep = keepPuk(*hmPuk_);

	Motors::instance().controlSwitch(Motor::State::OPEN);

	toSwitchClose_ = keep ? scKeep_ : scKick_;

	if(!keep)
	{
		puks_.erase(std::find(puks_.begin(), puks_.end(), hmPuk_));
	}

	hmPuk_.reset();
}

bool Running::keepPuk(Puk& puk)
{
	if(puk.upsideDown() || puk.type() == Puk::Type::UNKNOWN)
		return false;

	bool same = nextPuk_ == toType(puk);

	std::cout << "expected " << nextPuk_ << ", got " << toType(puk) << std::endl;

	if(same)
	{
		nextPuk_ = (nextPuk_ + 1) % CPUKS;
	}

	return same || Sensors::instance().rampFull() || puk.metal();
}

Running::iter_t Running::anyPukIn(uint32_t p, int f)
{
	for(iter_t i1 = puks_.begin(), i2 = puks_.end() ; i1 != i2 ; ++i1)
	{
		if((*i1)->isIn(p, f))
			return i1;
	}

	return puks_.end();
}

}

