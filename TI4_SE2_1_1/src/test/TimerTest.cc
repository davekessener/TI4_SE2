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

void init_logger(Logger_ptr l)
{
	l->addHandler(toHandler(StreamHandler(&std::cout), toFormatter(DefaultFormatter())));
}

void testfn(void)
{
	Logger &logger(*LogManager::instance().rootLog());

	MXT_LOG(LogLevel::INFO, "in testfn");

	timer->reset();
}

void timers(void)
{
	init_logger(LogManager::instance().rootLog());
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

