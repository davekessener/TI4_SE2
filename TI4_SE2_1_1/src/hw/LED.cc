#include <stdio.h>

#include "hw/LED.h"
#include "hw/Actuator.h"

namespace hw
{

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
	led_activate_packet p;

	p.cmd = Actuator::LED_ACTIVATE;
	p.led = led;
	p.f = f;

	blinkers_[get_led_id(led)].deactivate();

	con_.send(Packet_ptr(new DataPacket(p)));
}

void LED::blink(led_t led, const lib::Time& f)
{
	Lock guard(this);

	lib::Timer *t = &blinkers_[get_led_id(led)];
	t->reset();
	t->executeWhen(f, Ftor<LED, led_t>(this, &LED::blink_thread, led));
}

// # ---------------------------------------------------------------------------

void LED::doActivate(const void *d)
{
	const led_activate_packet *p = static_cast<const led_activate_packet *>(d);

	fprintf(stderr, "activate led %i 0x%08x\n", p->f, p->led);

	(HWAccess::instance().*(p->f ? &HWAccessImpl::setBits : &HWAccessImpl::resetBits))(MXT_PORT(p->led), MXT_PINS(p->led));
}

void LED::blink_thread(led_t led)
{
	int id = get_led_id(led);
	activate(led, blinkState_[id]);
	blinkState_[id] = !blinkState_[id];
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

