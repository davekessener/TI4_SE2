#include <iostream>

#include "hw/LED.h"
#include "lib/Data.h"

namespace hw
{

using lib::Data;

namespace
{
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

void LED::activate(led_t led, bool f)
{
	Lock guard(this);

	blinkers_[get_led_id(led)].deactivate();

	(HWAccess::instance().*(f ? &HWAccessImpl::setBits : &HWAccessImpl::resetBits))(MXT_PORT(led), MXT_PINS(led));
}

void LED::blink(led_t led, const lib::Time& f)
{
	Lock guard(this);

	blink_thread(led);
	blinkers_[get_led_id(led)].executeWhen(lib::Time::us(f.raw() / 2), Ftor<LED, led_t>(this, &LED::blink_thread, led));
}

// # ---------------------------------------------------------------------------

void LED::blink_thread(led_t led)
{
	Lock guard(this);

	int id = get_led_id(led);
	blinkState_[id] = !blinkState_[id];
	activate(led, blinkState_[id]);
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


