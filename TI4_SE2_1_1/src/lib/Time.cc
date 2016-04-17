#include <unistd.h>

#include "Time.h"

namespace lib
{

void Time::sleep(us_t v)
{
	usleep(v);
}

}

