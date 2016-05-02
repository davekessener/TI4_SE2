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

int isrId = 0;
int isrChannel = 0;
int isrConnection = 0;
struct sigevent isrEvent;

// ISR (c-Function)
const struct sigevent* ISR_DIO(void* arg, int id) {
    struct sigevent* event = (struct sigevent*) arg;
    int v = in8(0x318);

    event->sigev_notify = SIGEV_PULSE ;
    event->__sigev_un1.__sigev_coid = isrConnection;
    event->__sigev_un2.__st.__sigev_code = 0;
    event->sigev_value.sival_int = (v << 16) | in8(v == 2 ? 0x301 : 0x302);
    return event;
}


void registerISR(void){
    out8(0x30F, 0);
    out8(0x30B, 0b11111001);

    SIGEV_PULSE_INIT(&isrEvent, isrConnection, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    isrId = InterruptAttach(11, ISR_DIO, &isrEvent, sizeof(isrEvent), 0);
    if (isrId == -1) {
        exit(EXIT_FAILURE);
    }
}


void unregisterISR(void){
    if( InterruptDetach(isrId) == -1 ){
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


void isr() {
    // Init and Register ISR
    if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
        exit(EXIT_FAILURE);
    }

    if (( isrChannel = ChannelCreate(0)) == -1){
        exit(EXIT_FAILURE);
    }

    if ((isrConnection = ConnectAttach(0, 0, isrChannel, 0, 0)) == -1){
        exit(EXIT_FAILURE);
    }

    // Register Interupt Service Routine
    registerISR();

    struct _pulse pulse;
    int i = 0;

    // Wait for Pulse Message
    do{
        MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);

        // Print received Pulse message Value
        printf("int %04i: 0x%08x ", i++, pulse.value.sival_int);
        printb(pulse.value.sival_int);
        printf("\n");
//        std::cout << "Got an Interrupt, value: " << pulse.value.sival_int << std::endl;
    }while(1);

    // Cleanup
    unregisterISR();
    if( ConnectDetach(isrConnection) == -1){
        exit(EXIT_FAILURE);
    }

    if( ChannelDestroy(isrChannel) == -1 ){
        exit(EXIT_FAILURE);
    }
}

}

