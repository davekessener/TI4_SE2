#include "test/TimerTest.h"
#include "lib/log/Logger.h"
#include "lib/log/StreamHandler.h"
#include "lib/log/DefaultFormat.h"

namespace test
{

using namespace lib::log;
using lib::Timer;
using lib::Time;

Logger *log = NULL;
Timer *timer = NULL;

Logger *init_logger(Logger *l)
{
	l->addHandler(toHandler(StreamHandler(&std::cout), toFormatter(DefaultFormatter())));
	return l;
}

void testfn(void)
{
	Logger &logger(*log);

	MXT_LOG(LogLevel::INFO, "in testfn");

	timer->reset();
}

void timers(void)
{
	log = init_logger(new Logger);
	timer = new Timer;

	std::cout << "starting timer test" << std::endl;

	timer->executeWhen(Time::s(1), testfn);

	Time::ms(3500).wait();

	timer->deactivate();

	std::cout << "leaving now" << std::endl;

	delete timer; timer = NULL;
	delete log; log = NULL;
}

}

