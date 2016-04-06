#include <iostream>
#include <unistd.h>

#include <concurrent/Thread.h>
#include <HWAccess.h>

#include "LightsController.h"
#include "Lights.h"

int main(int argc, char *argv[])
{
	using SE::Lights;
	using SE::LightsController;

	std::cout << "Start blinking lights test." << std::endl;

	lib::Thread lc_thread(LightsController::light_controller_thread);

	LightsController& lights(LightsController::instance());

	std::cout << "thread created." << std::endl;

	lights.turnOn(Lights::RED);

	sleep(1);

	lights.turnOn(Lights::GREEN);

	sleep(1);

	lights.turnOff(Lights::RED);

	sleep(1);

	lights.turnOff(Lights::GREEN);

	std::cout << "joining ..." << std::endl;

	lights.stop();

	lc_thread.join();

	std::cout << "DONE" << std::endl;

	return 0;
}

