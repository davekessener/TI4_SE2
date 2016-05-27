#ifndef HAW_HW_LED_H
#define HAW_HW_LED_H

#include "hw/HWAccess.h"
#include "lib/macro.h"
#include "lib/TimeP.h"
#include "lib/Timer.h"
#include "lib/concurrent/Lock.hpp"

namespace hw
{
	class Actuator;

/** Allows access to LEDs.
 * Offers an interface for accessing LEDs on the hw unit.
 * Implements blinking functionality via lib::Timer.
 */
	class LED : public lib::LockableObject<LED>
	{
		public:
			typedef lib::LockableObject<LED> Super;
			typedef Super::Lock Lock;

			typedef uint32_t led_t;
			static const led_t GREEN  = MXT_PINPORT(HWAccessImpl::PORT_A, 0x20); //< port A pin 5
			static const led_t YELLOW = MXT_PINPORT(HWAccessImpl::PORT_A, 0x40); //< port A pin 6
			static const led_t RED    = MXT_PINPORT(HWAccessImpl::PORT_A, 0x80); //< port A pin 7
			static const led_t START  = MXT_PINPORT(HWAccessImpl::PORT_C, 0x01); //< port C pin 0
			static const led_t RESET  = MXT_PINPORT(HWAccessImpl::PORT_C, 0x02); //< port C pin 1
			static const led_t Q1     = MXT_PINPORT(HWAccessImpl::PORT_C, 0x04); //< port C pin 2
			static const led_t Q2     = MXT_PINPORT(HWAccessImpl::PORT_C, 0x08); //< port C pin 3
			static const int CLED = 7; //< Defines Size of LED Array

		public:
			void turnOn(led_t led) { activate(led, true); }
			void turnOff(led_t led) { activate(led, false); }
			void activate(led_t, bool);
			void blink(led_t, const lib::Time&);
		private:
			void blink_thread(led_t);
			static int get_led_id(led_t);

		private:
			lib::Timer blinkers_[CLED];
			bool blinkState_[CLED];

		private:
			LED( ) { }
			~LED( ) { }
			LED(const LED&);
			LED& operator=(const LED&);

			friend class Actuator;
	};
}

#endif

