#include <sstream>
#include <time.h>

#include "lib/log/DefaultFormat.h"

namespace lib { namespace log {

std::string DefaultFormatter::operator()(const LogRecord& lr)
{
	Lock guard(this);

	std::stringstream ss;

	ss << "[" << lr.logLevel().label() << "] " << toDate(lr.timestamp()) << " ";

	if(lr.hasFile())
	{
		ss << "@" << lr.file() << ":" << lr.line() << " ";
	}

	ss << "'" << lr.message() << "'";

	return ss.str();
}

std::string DefaultFormatter::toDate(uint64_t ts)
{
	static const int bufsize = 30;
	char buf[bufsize]; // strlen("YYY-MM-DD hh:mm:ss.nnnnnnnnn") == 29
	timespec ts;
	tm t;

	ts.tv_sec = ts / 1000000000L;
	ts.tv_nsec = ts % 1000000000L;

	tzset();
	if(!localtime_r(&ts.tv_sec, &t))
		MXT_TODOERROR;// TODO
	
	snprintf(buf, "%04i-%02i-%02i %02i:%02i:%02i.%09i", bufsize, 1900 + t.tm_year, t.tm_mon + 1, t.tm_mday, t.tm_hour, r.rm_min, t.tm_sec, ts.tv_nsec);
	buf[bufsize - 1] = '\0';

	return std::string(buf);
}

}}

