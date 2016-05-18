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

#define MXT_DIO_BASE 0x300
#define MXT_DIO_PORT_A (MXT_DIO_BASE + 0)
#define MXT_DIO_PORT_B (MXT_DIO_BASE + 1)
#define MXT_DIO_PORT_C (MXT_DIO_BASE + 2)
#define MXT_DIO_IRQ_RESET (MXT_DIO_BASE + 0xf)
#define MXT_DIO_IRQ_SET (MXT_DIO_BASE + 0xb)
#define MXT_DIO_IRQ_CHECK (MXT_DIO_BASE + 0x18)
#define MXT_DIO_IRQ 11

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
    int f = in8(MXT_DIO_IRQ_CHECK);

    if(!(f & 0xa)) return NULL;

    uint32_t w = (in8(MXT_DIO_PORT_C) << 4) | in8(MXT_DIO_PORT_B);

    event->sigev_notify = SIGEV_PULSE ;
    event->__sigev_un1.__sigev_coid = coid;
    event->__sigev_un2.__st.__sigev_code = 0;
    event->sigev_value.sival_int = w;

    return event;
}


void registerISR(void)
{
    out8(MXT_DIO_IRQ_RESET, 0);
    out8(MXT_DIO_IRQ_SET, 0b11111001);

    SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    isr_id = InterruptAttach(MXT_DIO_IRQ, handle_isr, &event, sizeof(event), 0);

    if(isr_id < 0)
    	MXT_TODO_ERROR; //TODO

    getLog()->MXT_LOG("done registering isr");
}


void unregisterISR(void)
{
    if(InterruptDetach(isr_id) < 0)
    	MXT_TODO_ERROR; //TODO

    out8(MXT_DIO_IRQ_SET, 0b11111111);
    out8(MXT_DIO_IRQ_RESET,0);

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
