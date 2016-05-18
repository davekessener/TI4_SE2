#include "test/SensorTest.h"

#include "hw/HWAccess.h"

#include "hw/Sensor.h"
#include "lib/log/LogManager.h"
#include "lib/mpl/fsm.hpp"

namespace test
{
	void sensors( )
	{
		using lib::Time;
		using lib::log::Logger_ptr;
		using lib::log::LogManager;
		using hw::LEDs;
		using hw::LED;
		using hw::Sensor;

		Time delay = Time::s(3);
		Logger_ptr log = LogManager::instance().rootLog();

		log->MXT_LOG("starting Sensor ISR FSM");
        Store s;
        MyFSM fsm(&s);

        #define PR(e) s.setNextEvent(#e); fsm.process(e())
            while(!enteringChanged) {}
            PR(ELB_01);
            while(!inHMChanged) {}
            PR(EHM);
            PR(ELB_02);
            PR(EMD);
            PR(ELB_03);
        #undef PR

	}
}
