#include "test/TimerTest.h"
#include "lib/log/Logger.h"
#include "lib/log/LogManager.h"
#include "lib/log/StreamHandler.h"
#include "lib/log/DefaultFormat.h"
#include "lib/TimeP.h"
#include "lib/Timer.h"

namespace test
{

using namespace lib::log;
using lib::Timer;
using lib::Time;

Timer *timer = NULL;

void testfn(void)
{
	Logger &logger(*LogManager::instance().rootLog());

	logger.MXT_LOGL(LogLevel::INFO, "in %s", "testfn");

	timer->reset();
}

void timers(void)
{
	timer = new Timer;

	std::cout << "starting timer test" << std::endl;

	timer->executeWhen(Time::s(1), testfn);

	Time::ms(3500).wait();

	timer->deactivate();

	std::cout << "leaving now" << std::endl;

	delete timer; timer = NULL;

	Time::s(1).wait();
}

}

