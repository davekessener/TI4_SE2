#include <iostream>

#include "test/ActuatorTest.h"
#include "test/TimerTest.h"
#include "test/SerialTest.h"
#include "test/SensorTest.h"

#include "run/Project.h"

int main(int argc, char *argv[])
{
//	test::actuators();
//	test::connections();
//	test::timers();
//	test::sensors();

	haw::Project p;

	p.run();

	return 0;
}
