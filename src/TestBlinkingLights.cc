#include <iostream>

#include <concurrent/Thread.h>

#include "LightsController.h"

int main(int argc, char *argv[])
{
	using SE::Lights;
	using SE::LightsController;

	lib::Thread lc_thread(LightsController::light_controller_thread);

	LightsController& lights(LightsController::instance());

	lights.turnOn(Lights::RED);

	sleep(1);

	lights.turnOn(Lights::GREEN);

	sleep(1);

	lights.turnOff(Lights::RED);

	sleep(1);

	lights.turnOff(Lights::GREEN);

	lights.stop();

	lc_thread.join();

	return 0;
}

