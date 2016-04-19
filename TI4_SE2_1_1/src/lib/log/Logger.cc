#include "lib/log/Logger.h"

namespace lib { namespace log {

// # ---------------------------------------------------------------------------

void Logger::addParent(Logger& p)
{
	Lock guard(this);

	parents_.push_back(&p);
}

void Logger::removeParent(Logger& p)
{
	Lock guard(this);

	parents_.erase(std::find(parents_.begin(), parents_.end(), &p));
}

void Logger::addHandler(Handler_ptr p)
{
	Lock guard(this);

	handlers_.push_back(p);
}

void Logger::removeHandler(Handler_ptr p)
{
	Lock guard(this);

	handlers_.erase(std::find(handlers_.begin(), handlers_.end(), p));
}

void Logger::addFilter(Filter_ptr p)
{
	Lock guard(this);

	filters_.push_back(p);
}

void Logger::removeFilter(Filter_ptr p)
{
	Lock guard(this);

	filters_.erase(std::find(filters_.begin(), filters_.end(), p));
}

// # ---------------------------------------------------------------------------

void Logger::log(const LogLevel& ll, const std::string& msg, const char *s, int l)
{
	log(LogRecord(ll, Timer::timestamp(), msg, s, l));
}

void Logger::log(const LogRecord& lr)
{
	Lock guard(this);

	for(fiter_t i1 = filters_.begin, i2 = filters_.end() ; i1 != i2 ; ++i1)
	{
		if(!(*i1)->accept(lr)) return;
	}

	for(hiter_t i1 = handlers_.begin(), i2 = handlers_.end() ; i1 != i2 ; ++i1)
	{
		(*i1)->handle(lr);
	}

	for(piter_t i1 = parents_.begin(), i2 = parents_.end() ; i1 != i2 ; ++i1)
	{
		(*i1)->log(lr);
	}
}

}}

