#include <vector>

#include "run/Project.h"
#include "run/State.h"

namespace haw
{
using lib::Time;
using lib::Speed;

void Project::run(void)
{
	std::vector<State_ptr> states;
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

