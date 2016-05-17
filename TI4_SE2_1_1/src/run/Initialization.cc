#include "run/Initialization.h"

#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "hw/Sensor.h"
#include "hw/Actuator.h"
#include "hw/LED.h"
#include "hw/Motor.h"
#include "run/Puk.h"

namespace haw
{

using hw::LEDs;
using hw::LED;
using hw::Sensors;
using hw::Motors;
using hw::Motor;
using lib::Frequency;

// # ===========================================================================

void Calibrator::Measurer::tick(void)
{
	uint16_t h = Sensors::instance().getHeight();

	if(h > height_)
		height_ = h;
}

void Calibrator::Measurer::start(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::ENTERING && e.value())
	{
		Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::FAST);
		state_ = &Measurer::toHM;
	}
}

void Calibrator::Measurer::toHM(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::IN_HM && e.value())
	{
		Motors::instance().controlBelt(Motor::Direction::LEFT, Motor::Speed::FAST);
		state_ = &Measurer::back;
	}
}

void Calibrator::Measurer::back(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::ENTERING && e.value())
	{
		Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
		state_ = &Measurer::end;
	}
}

// # ===========================================================================

void Calibrator::start(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::START && e.value())
	{
		LEDs::instance().turnOn(LED::GREEN);
		std::cout << "-> please enter a flat puk" << std::endl;
		state_ = &Calibrator::measureFlat;
		meas_.reset();
	}
}

void Calibrator::measureFlat(const SensorEvent& e)
{
	meas_.process(e);

	if(meas_.done())
	{	
		min_ = meas_.getHeight();
		std::cout << "-> please enter a large puk upside-down" << std::endl;
		state_ = &Calibrator::measureLarge;
		meas_.reset();
	}
}

void Calibrator::measureLarge(const SensorEvent& e)
{
	meas_.process(e);

	if(meas_.done())
	{
		max_ = meas_.getHeight();
		std::cout << "measured min " << min_ << ", max " << max_ << std::endl;
		project_.calibrateHM(min_, max_);
		std::cout << "-> please enter a metallic puk right-side up" << std::endl;
		state_ = &Calibrator::startMain;
	}
}

void Calibrator::startMain(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::ENTERING && e.value())
	{
		Motors::instance().controlBelt(Motor::Direction::RIGHT, Motor::Speed::SLOW);
		state_ = &Calibrator::toHM;
	}
}

void Calibrator::toHM(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::ENTERING && !e.value())
	{
		timer_.reset();
	}

	if(e.sensor() == SensorEvent::Sensors::IN_HM)
	{
		if(e.value())
		{
			toHM_ = timer_.delta();
			project_.startHM();
		}
		else
		{
			puk_ = timer_.delta();
			
			int t = project_.stopHM();
			if(t != Puk::Type::LARGE)
			{
				std::cout << "type read: " << t << std::endl;
				MXT_TODO_ERROR; //TODO
			}
			
			state_ = &Calibrator::toSwitch;
		}
	}
}

void Calibrator::toSwitch(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::IN_SWITCH && e.value())
	{
		if(!Sensors::instance().isMetal())
			MXT_TODO_ERROR; //TODO

		Motors::instance().controlSwitch(Motor::State::OPEN);
		state_ = &Calibrator::toEnd;
	}
}

void Calibrator::toEnd(const SensorEvent& e)
{
	if(e.sensor() == SensorEvent::Sensors::LEAVING)
	{
		if(e.value())
		{
			slow_ = timer_.delta() + toHM_ + puk_;
			Motors::instance().controlBelt(Motor::Direction::LEFT, Motor::Speed::FAST);
		}
		else
		{
			timer_.reset();
		}
	}

	if(e.sensor() == SensorEvent::Sensors::ENTERING && e.value())
	{
		fast_ = timer_.delta();
		project_.calibrateDistances(slow_, fast_, toHM_, puk_);
		Motors::instance().controlSwitch(Motor::State::CLOSE);
		Motors::instance().controlBelt(Motor::Direction::NONE, Motor::Speed::STOP);
		LEDs::instance().turnOff(LED::GREEN);
		std::cout << "[DONE] calibrating" << std::endl;
		std::cout << "Please remove puk, then press START to begin" << std::endl;
		state_ = &Calibrator::end;
	}
}

// # ===========================================================================

void Initialization::enter(void)
{
	hw::HWAccess::instance();
	hw::Sensors::instance();
	hw::Actuators::instance();
	hw::LEDs::instance();
	hw::Motors::instance();

	LEDs::instance().turnOn(LED::YELLOW);

	std::cout << "Please press START to calibrate" << std::endl;
}

void Initialization::exit(void)
{
	LEDs::instance().turnOff(LED::YELLOW);
}

void Initialization::process(const Event& e)
{
	if(e.id() == Event::ID::SENSOR)
	{
		const SensorEvent &se = *reinterpret_cast<const SensorEvent *>(&e);

		cali_.process(se);

		switch(se.sensor())
		{
//			case SensorEvent::Sensors::ESTOP:
			case SensorEvent::Sensors::STOP:
				if(se.value())
					MXT_TODO_ERROR; //TODO
				break;
			case SensorEvent::Sensors::START:
				if(cali_.done() && se.value())
				{
					LEDs::instance().turnOff(LED::YELLOW);
					LEDs::instance().blink(LED::GREEN, Frequency::Hz(0.5));
					std::cout << "Entering READY state" << std::endl;
					setNext(State::NEXT);
				}
				break;
		};
	}
}

// # ===========================================================================

}

