#include <iostream>
#include <cstdlib>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <hw/inout.h>
#include <stdio.h>

#include "hw/Sensor.h"
#include "lib/log/LogManager.h"
#include "lib/macro.h"
#include "lib/qnx/ISR.h"

using lib::log::Logger_ptr;

Logger_ptr getLog(void)
{
	static Logger_ptr log = lib::log::LogManager::instance().getLog(MXT_LOG_ISR);

	return log;
}

// DIO48-S
// base: 300h
// irq:   11d

// AIO12-8
// base: 320h
// irq:   14d

int isr_id = 0;
int chid = 0;
int coid = 0;
struct sigevent event;

const struct sigevent* handle_isr(void* arg, int id)
{
    struct sigevent* event = (struct sigevent*) arg;
    out8(0x30f, 0);
    uint32_t w = (in8(0x302) << 4) | in8(0x301);

    event->sigev_notify = SIGEV_PULSE ;
    event->__sigev_un1.__sigev_coid = coid;
    event->__sigev_un2.__st.__sigev_code = 0;
    event->sigev_value.sival_int = w;

    return event;
}


void registerISR(void)
{
    out8(0x30F, 0);
    out8(0x30B, 0b11111001);

    SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    isr_id = InterruptAttach(11, handle_isr, &event, sizeof(event), 0);

    if(isr_id < 0)
    	MXT_TODO_ERROR; //TODO

    getLog()->MXT_LOG("done registering isr");
}


void unregisterISR(void)
{
    if(InterruptDetach(isr_id) < 0)
    	MXT_TODO_ERROR; //TODO

    out8(0x30B, 0b11111111);
    out8(0x30F,0);

    getLog()->MXT_LOG("done unregistering isr");
}

void isr_start(void)
{
    if(ThreadCtl(_NTO_TCTL_IO_PRIV, 0) < 0)
    	MXT_TODO_ERROR; //TODO

    if((chid = ChannelCreate(0)) < 0)
    	MXT_TODO_ERROR; //TODO

    if((coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0)) < 0)
    	MXT_TODO_ERROR; //TODO
}

void isr_stop(void)
{
    if(ConnectDetach(coid) < 0)
    	MXT_TODO_ERROR; //TODO

    if(ChannelDestroy(chid) < 0)
    	MXT_TODO_ERROR; //TODO
}

void isr_main(void)
{
	isr_start();

    registerISR();

    struct _pulse pulse;

    while(true)
    {
        if(MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL) < 0)
        	break;

        hw::Sensors::instance().handlePulse(pulse.value.sival_int);
    }

    unregisterISR();
}
