#include "hw/Sensor.h"

#include "lib/TimeP.h"
#include "hw/HWAccess.h"
#include "lib/log/LogManager.h"

namespace hw
{

namespace
{
	uint32_t dio_isr(void)
	{
		HWAccess::instance().out(HWAccessImpl::DIO_IRQ_RESET, 0);

		return 0;
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
	hw::HWAccess::instance();
	lib::log::Logger_ptr log = lib::log::LogManager::instance().rootLog();
	lib::qnx::Receiver recv = ch_.open();
	con_ = ch_.connect();
	running_ = true;

	initISR();

	log->MXT_LOG("done initializing isr");

	try
	{
		while(running_)
		{
			log->MXT_LOG("waiting for pulse");
			handlePulse(recv.getPulse());
			log->MXT_LOG("handled pulse");
		}
	}
	catch(...)
	{
		running_ = false;
	}

	log->MXT_LOG("cleaning up isr");

	cleanupISR();
}

void Sensor::handlePulse(uint32_t pulse)
{
	HWAccessImpl &hal = HWAccess::instance();
	uint32_t v = ((hal.in(HWAccessImpl::PORT_C) & 0xf0) << 4) | hal.in(HWAccessImpl::PORT_B);

	lib::log::LogManager::instance().rootLog()->MXT_LOG("received pulse");

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
	lib::log::LogManager::instance().rootLog()->MXT_LOG("wrote 0 to 0x30f");
	HWAccess::instance().out(HWAccessImpl::DIO_IRQ_RESET, 0);
	lib::log::LogManager::instance().rootLog()->MXT_LOG("wrote 0b1111 1001 to 0x30b");
	HWAccess::instance().out(HWAccessImpl::DIO_IRQ_MASK, 0b11111001);

	con_.registerISR(HWAccessImpl::DIO_IRQ, dio_isr);
}

void Sensor::cleanupISR(void)
{
	con_.close();

	HWAccess::instance().out(HWAccessImpl::DIO_IRQ_MASK, 0b1111111);
	HWAccess::instance().out(HWAccessImpl::DIO_IRQ_RESET, 0);
}

}

