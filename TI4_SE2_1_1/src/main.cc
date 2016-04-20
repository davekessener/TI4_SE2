#include <iostream>
#include <stdexcept>
#include "lib/TimeP.h"
#include "test/ActuatorTest.h"
#include "test/TimerTest.h"
#include "test/SerialTest.h"
#include "lib/concurrent/Thread.h"

int main(int argc, char *argv[])
{
	test::actuators();
	test::serial();

	return 0;
}
