#include <iostream>

#include "lib/log/StreamHandler.h"

namespace lib { namespace log {

void StreamHandler::operator()(const std::string& r)
{
	Lock guard(this);

	std::string s(r);
	s.append("\n");

	(*os_) << s;
}

}}

