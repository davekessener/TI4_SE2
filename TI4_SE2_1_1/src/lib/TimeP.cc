#include <unistd.h>

#include "lib/TimeP.h"

namespace lib
{

void Time::toTimespec(timespec *ts)
{
	clock_gettime(CLOCK_REALTIME, ts);
	ts->tv_sec += t_ / 1000000L;
	ts->tv_nsec += (t_ % 1000000L) * 1000L;
	ts->tv_sec += ts->tv_nsec / 1000000000L;
	ts->tv_nsec %= 1000000000L;
}

void Time::sleep(us_t v)
{
	usleep(v);
}

}

