#include "hw/Sensor.h"

#include "lib/TimeP.h"

namespace hw
{

namespace
{
	uint32_t dio_isr(void)
	{
		using HWAccessImpl::DIO_IRQ_RESET;

		HWAccess::instance().out(DIO_IRQ_RESET, 0);
	}
}

Sensor::Sensor(void)
{
	for(int i = 0 ; i < VAR_COUNT ; ++i)
	{
		vals_[i] = flags_[i] = false;
	}

	thread_.reset(new lib::Thread(lib::wrapInFtor(this, &Sensor::thread)));

	while(!ch_.isOpen()) lib::Time::ms(2).wait();
}

Sensor::~Sensor(void)
{
	if(static_cast<bool>(thread_.get()) && thread_->joinable())
		thread_->join();
}

void Sensor::thread(void)
{
	lib::qnx::Receiver recv = ch_.open();
	con_ = ch_.connect();
	running_ = true;

	initISR();

	try
	{
		while(running_)
		{
			handlePulse(recv.getPulse());
		}
	}
	catch(...)
	{
		running_ = false;
	}

	cleanupISR();
}

void Sensor::handleISR(uint32_t pulse)
{
	using HWAccessImpl::PORT_B;
	using HWAccessImpl::PORT_C;

	HWAccessImpl &hal = HWAccess::instance();
	uint32_t v = ((hal.in(PORT_C) & 0xf0) << 4) | hal.in(PORT_B);

	for(int i = 0 ; i < VAR_COUNT ; ++i)
	{
		bool y = v & (1 << i);

		if(y != vals_[i])
		{
			vals_[i] = y;
			flags_[i] = true;
		}
	}
}

void Sensor::initISR(void)
{
	using HWAccessImpl::DIO_IRQ_MASK;
	using HWAccessImpl::DIO_IRQ_RESET;
	using HWAccessImpl::DIO_IRQ;

	HWAccess::instance().out(DIO_IRQ_RESET, 0);
	HWAccess::instance().out(DIO_IRQ_MASK, 0b11111001);

	con_.registerISR(DIO_IRQ, dio_isr);
}

void Sensor::cleanupISR(void)
{
	using HWAccessImpl::DIO_IRQ_MASK;
	using HWAccessImpl::DIO_IRQ_RESET;
	using HWAccessImpl::DIO_IRQ;

	con_.close();

	HWAccess::instance().out(DIO_IRQ_MASK, 0b1111111);
	HWAccess::instance().out(DIO_IRQ_RESET, 0);
}

}

