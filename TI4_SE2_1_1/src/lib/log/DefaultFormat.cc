#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "lib/macro.h"
#include "lib/log/DefaultFormat.h"

namespace lib { namespace log {

namespace
{
	std::string printTID(uint16_t tid)
	{
		static const int bufsize = 2 * sizeof(uint16_t) + 1;
		char buf[bufsize];

		snprintf(buf, bufsize, "%04x", tid);

		buf[bufsize-1] = '\0';

		return std::string(buf);
	}
}

std::string DefaultFormatter::operator()(const LogRecord& lr)
{
	Lock guard(this);

	std::stringstream ss;

	ss << printTID(lr.threadID()) << " [" << lr.logLevel().label() << "] " << toDate(lr.timestamp()) << " ";

	if(lr.hasFile())
	{
		ss << "@" << lr.file() << ":" << lr.line() << " ";
	}

	ss << "'" << lr.message() << "'";

	return ss.str();
}

std::string DefaultFormatter::toDate(uint64_t timestamp)
{
	static const int bufsize = 30;
	char buf[bufsize]; // strlen("YYY-MM-DD hh:mm:ss.nnnnnnnnn") == 29
	timespec ts;
	tm t;

	ts.tv_sec = timestamp / 1000000000L;
	ts.tv_nsec = timestamp % 1000000000L;

	tzset();
	if(!localtime_r(&ts.tv_sec, &t))
		MXT_TODO_ERROR;// TODO
	
	snprintf(buf, bufsize, "%04i-%02i-%02i %02i:%02i:%02i.%09i", 1900 + t.tm_year, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, (int) ts.tv_nsec);
	buf[bufsize - 1] = '\0';

	return std::string(buf);
}

}}

