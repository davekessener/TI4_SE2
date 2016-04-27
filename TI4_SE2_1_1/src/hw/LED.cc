#include <iostream>

#include "hw/LED.h"
#include "hw/Actuator.h"
#include "lib/Data.h"

namespace hw
{

using lib::Data;

namespace
{
	struct led_activate_packet
	{
		uint8_t cmd;
		LED::led_t led;
		bool f;
	};

	template<typename T, typename TT>
	struct Ftor
	{
		typedef void (T::*f_fn)(TT);
		Ftor(T *e, f_fn f, TT a) : e_(e), f_(f), a_(a) { }
		void operator()( ) { (e_->*f_)(a_); }
		T *e_;
		f_fn f_;
		TT a_;
	};
}

LED::LED(void) : con_(Actuators::instance().getChannel().connect())
{
}

void LED::activate(led_t led, bool f)
{
	Lock guard(this);

	blinkers_[get_led_id(led)].deactivate();

	sendActivate(led, f);
}

void LED::blink(led_t led, const lib::Time& f)
{
	Lock guard(this);

	blink_thread(led);
	blinkers_[get_led_id(led)].executeWhen(lib::Time::us(f.raw() / 2), Ftor<LED, led_t>(this, &LED::blink_thread, led));
}

// # ---------------------------------------------------------------------------

void LED::sendActivate(led_t led, bool f)
{
	Lock guard(this);

	led_activate_packet p;

	p.cmd = Actuator::LED_ACTIVATE;
	p.led = led;
	p.f = f;

	con_.send(Data::get(p));
}

void LED::doActivate(const void *d)
{
	const led_activate_packet *p = static_cast<const led_activate_packet *>(d);

	(HWAccess::instance().*(p->f ? &HWAccessImpl::setBits : &HWAccessImpl::resetBits))(MXT_PORT(p->led), MXT_PINS(p->led));
}

void LED::blink_thread(led_t led)
{
	Lock guard(this);

	int id = get_led_id(led);
	blinkState_[id] = !blinkState_[id];
	sendActivate(led, blinkState_[id]);
	blinkers_[id].reset();
}

int LED::get_led_id(led_t led)
{
	switch(led)
	{
		case GREEN:  return 0;
		case YELLOW: return 1;
		case RED:    return 2;
		case START:  return 3;
		case RESET:  return 4;
		case Q1:     return 5;
		case Q2:     return 6;
	}

	MXT_TODO_ERROR;

	return -1;
}

}

