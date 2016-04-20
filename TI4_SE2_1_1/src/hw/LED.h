#ifndef HAW_HW_LED_H
#define HAW_HW_LED_H

#include <hw/HWAccess.h>
#include <lib/macro.h>
#include <lib/TimeP.h>
#include <lib/Timer.h>
#include <lib/Singleton.hpp>
#include <lib/qnx/Channel.h>
#include <lib/concurrent/Lock.hpp>

namespace hw
{
	class Actuator;

	class LED : public lib::LockableObject<LED>
	{
		public:
			typedef lib::LockableObject<LED> Super;
			typedef lib::Singleton<LED, lib::SingletonConcurrency::MultiThreaded> SingletonInst;
			typedef Super::Lock Lock;

			typedef uint32_t led_t;
			static const led_t GREEN  = MXT_PINPORT(HWAccessImpl::PORT_A, 0x20); // port A pin 5
			static const led_t YELLOW = MXT_PINPORT(HWAccessImpl::PORT_A, 0x40); // port A pin 6
			static const led_t RED    = MXT_PINPORT(HWAccessImpl::PORT_A, 0x80); // port A pin 7
			static const led_t START  = MXT_PINPORT(HWAccessImpl::PORT_C, 0x01); // port C pin 0
			static const led_t RESET  = MXT_PINPORT(HWAccessImpl::PORT_C, 0x02); // port C pin 1
			static const led_t Q1     = MXT_PINPORT(HWAccessImpl::PORT_C, 0x04); // port C pin 2
			static const led_t Q2     = MXT_PINPORT(HWAccessImpl::PORT_C, 0x08); // port C pin 3
			static const int CLED = 7;

		public:
			void turnOn(led_t led) { activate(led, true); }
			void turnOff(led_t led) { activate(led, false); }
			void activate(led_t, bool);
			void blink(led_t, const lib::Time&);
		private:
			void sendActivate(led_t, bool);
			void doActivate(const void *);
			void blink_thread(led_t);
			static int get_led_id(led_t);

		private:
			lib::qnx::Connection con_;
			lib::Timer blinkers_[CLED];
			bool blinkState_[CLED];

		public:
			LED( );
			~LED( ) { }
		private:
			LED(const LED&);
			LED& operator=(const LED&);

			friend class Actuator;
	};

	typedef LED::SingletonInst LEDs;
}

#endif

