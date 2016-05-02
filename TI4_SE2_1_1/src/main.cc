#include <iostream>

#include "test/ActuatorTest.h"
#include "test/TimerTest.h"
#include "test/SerialTest.h"
#include "test/ISR.h"

int main(int argc, char *argv[])
{
//	test::actuators();
//	test::connections();
	test::isr();

	return 0;
}
