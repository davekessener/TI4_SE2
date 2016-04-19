#include <iostream>

#include "lib/log/LogManager.h"
#include "lib/log/StreamHandler.h"
#include "lib/log/DefaultFormat.h"

namespace lib { namespace log {

LogManagerImpl::LogManagerImpl(void) : root_(new Logger)
{
	root_->addHandler(toHandler(StreamHandler(&std::cout), toFormatter(DefaultFormatter())));
}

Logger_ptr LogManagerImpl::getLog(const std::string& s)
{
	Lock guard(this);

	Logger_ptr l = logs_[s];

	if(!static_cast<bool>(l))
	{
		l.set(new Logger);
	}

	return l;
}

}}

