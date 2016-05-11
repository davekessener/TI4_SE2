#include "hw/Sensor.h"
#include "lib/Timer.h"
#include "lib/TimeP.h"
#include "lib/log/LogManager.h"
#include "hw/HWAccess.h"

#include "test/ISR.h"

#define isr_old isr

namespace test {

void isr_new(void)
{
	using hw::Sensors;
	using hw::Sensor;
	using lib::Timer;
	using lib::Time;
	using lib::log::LogManager;
	using lib::log::Logger_ptr;

	Sensor &sensors = Sensors::instance();
	Logger_ptr log = LogManager::instance().rootLog();
	Timer fps;
	Time delay = Time::ms(1);

	log->MXT_LOG("Starting sensors loop");

	while(true)
	{
		if(sensors.leavingChanged())
		{
			log->MXT_LOG("LEAVING is %s", (sensors.leaving() ? "on" : "off"));
		}

		if(sensors.enteringChanged())
		{
			log->MXT_LOG("ENTERING caused quit");
			break;
		}

		fps.sync(delay);
	}
}

}

//#define DO_LOG(...) lib::log::LogManager::instance().rootLog()->MXT_LOG(__VA_ARGS__)
#define DO_LOG(...)

#include <iostream>
#include <cstdlib>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <hw/inout.h>
#include <stdio.h>

// DIO48-S
// base: 300h
// irq:   11d

// AIO12-8
// base: 320h
// irq:   14d

#include "test/ISR.h"

namespace test
{

int isr_id = 0;
int chid = 0;
int coid = 0;
struct sigevent event;

// ISR (c-Function)
const struct sigevent* handle_isr(void* arg, int id) {
    struct sigevent* event = (struct sigevent*) arg;
    int v = in8(0x318);

    event->sigev_notify = SIGEV_PULSE ;
    event->__sigev_un1.__sigev_coid = coid;
    event->__sigev_un2.__st.__sigev_code = 0;
    event->sigev_value.sival_int = (v << 16) | in8(v == 2 ? 0x301 : 0x302);
    return event;
}


void registerISR(void){
	DO_LOG("write 0 to 0x30f");
    out8(0x30F, 0);
    DO_LOG("write 0b1111 1001 to 0x30b");
    out8(0x30B, 0b11111001);

    DO_LOG("SIGEV_PULSE_INIT");
    SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    DO_LOG("InterruptAttach");
    isr_id = InterruptAttach(11, handle_isr, &event, sizeof(event), 0);
    if (isr_id == -1) {
        exit(EXIT_FAILURE);
    }
}


void unregisterISR(void){
    if( InterruptDetach(isr_id) == -1 ){
        exit(EXIT_FAILURE);
    }
    out8(0x30B, 0b11111111);
    out8(0x30F,0);
}

void printb(int v)
{
	int i;
	for(i = 7 ; i >= 0 ; --i)
	{
		printf("%c", (v & (1 << i)) ? '1' : '0');
	}
}

void isr_old() {
	DO_LOG("calling thread ctrl");
    if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
        exit(EXIT_FAILURE);
    }

	DO_LOG("create channel");
    if (( chid = ChannelCreate(0)) == -1){
        exit(EXIT_FAILURE);
    }

	DO_LOG("create connection");
    if ((coid = ConnectAttach(0, 0, chid, 0, 0)) == -1){
        exit(EXIT_FAILURE);
    }

    // Register Interupt Service Routine
    registerISR();

    struct _pulse pulse;
    int i = 0;

    // Wait for Pulse Message
    do{
    	DO_LOG("started MsgReceivePulse");
        MsgReceivePulse(chid,&pulse,sizeof(pulse),NULL);
    	DO_LOG("receiveded MsgReceivePulse");

        // Print received Pulse message Value
        printf("int %04i: 0x%08x ", i++, pulse.value.sival_int);
        printb(pulse.value.sival_int);
        printf("\n");
//        std::cout << "Got an Interrupt, value: " << pulse.value.sival_int << std::endl;
    }while(1);

    // Cleanup
    unregisterISR();
    if( ConnectDetach(coid) == -1){
        exit(EXIT_FAILURE);
    }

    if( ChannelDestroy(chid) == -1 ){
        exit(EXIT_FAILURE);
    }
}

}

