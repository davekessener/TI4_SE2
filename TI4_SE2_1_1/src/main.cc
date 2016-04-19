#include <iostream>
#include <stdexcept>
#include "lib/TimeP.h"
#include "test/LEDTest.h"
#include "test/TimerTest.h"
#include "test/SerialTest.h"
#include "lib/concurrent/Thread.h"

int main(int argc, char *argv[])
{
	lib::Thread ser1(test::serialA), ser2(test::serialB);

	ser1.join();
	ser2.join();

	return 0;
}
