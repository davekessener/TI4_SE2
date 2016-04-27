#include <iostream>

#include "test/ActuatorTest.h"
#include "test/TimerTest.h"
#include "test/SerialTest.h"

int main(int argc, char *argv[])
{
	test::actuators();
	test::connections();

	return 0;
}
