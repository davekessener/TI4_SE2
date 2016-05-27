#include <sys/neutrino.h>
#include <hw/inout.h>

#include "hw/Sensor.h"
#include "hw/HWAccess.h"
#include "lib/qnx/ISR.h"
#include "lib/TimeP.h"
#include "lib/Timer.h"
#include "lib/log/LogManager.h"

namespace hw
{

Sensor::Sensor(void) : running_(false), height_(0)
{
	for(int i = 0 ; i < VAR_COUNT ; ++i)
	{
		vals_[i] = false;
	}

	hm_thread_.reset(new lib::Thread(lib::wrapInFtor(this, &Sensor::manageHM)));
	isr_thread_.reset(new lib::Thread(isr_main));
}

Sensor::~Sensor(void)
{
	shutdown();
}

void Sensor::shutdown(void)
{
	if(static_cast<bool>(isr_thread_.get()))
	{
		running_ = false;

		isr_stop();

		isr_thread_->join();
		hm_thread_->join();

		isr_thread_.reset();
		hm_thread_.reset();
	}
}

uint16_t Sensor::readHM(void)
{
	HWAccessImpl &hw = HWAccess::instance();
	uint16_t height;

	height = hw.in(HWAccessImpl::AIO_LOW);
	hw.out(HWAccessImpl::AIO_CONVERT, HWAccessImpl::AIO_START_CONVERSION);

	lib::Time::us(10).wait();

	height |= (uint16_t)hw.in(HWAccessImpl::AIO_HIGH) << 8;

	return height & 0xfff;
}

void Sensor::manageHM(void)
{
	lib::Time delay = lib::Time::us(200);
	lib::Timer fps;

	running_ = true;

	HWAccess::instance().initThread();

	while(running_)
	{
		uint16_t h = readHM();

		height_ = ~h;

		fps.sync(delay);
	}
}

void Sensor::handlePulse(uint32_t pulse)
{
	Lock guard(this);
	//! Holds the current pulse. Sensor signals are bit flipped to 0 when not activated.
	uint32_t v = pulse ^ 0x0acb; // 0b1010 1100 1011

	for(int i = 0 ; i < VAR_COUNT ; ++i)
	{
		bool y = v & (1 << i);

		if(y != vals_[i])
		{
			vals_[i] = y;
			notify(i);
		}
	}
}

}


