#ifndef HAW_HW_LED_H
#define HAW_HW_LED_H

#include <Time.h>
#include <Timer.h>
#include <Singleton.hpp>
#include <qnx/Channel.h>
#include <concurrent/Lock.hpp>

namespace hw
{
	class Actuator;

	class LED : public lib::LockableObject<LED>
	{
		public:
			using HWAccess::PORT_A, HWAccess::PORT_C;

			typedef lib::LockableObject<LED> Super;
			typedef lib::Singleton<LED, lib::SingletonConcurrency::MultiThreaded> SingletonInst;
			typedef Super::Lock Lock;

			typedef uint32_t led_t;
			static const led_t GREEN  = MXT_PINPORT(PORT_A, 0x20); // port A pin 5
			static const led_t YELLOW = MXT_PINPORT(PORT_A, 0x40); // port A pin 6
			static const led_t RED    = MXT_PINPORT(PORT_A, 0x80); // port A pin 7
			static const led_t START  = MXT_PINPORT(PORT_C, 0x00); // port C pin 0
			static const led_t RESET  = MXT_PINPORT(PORT_C, 0x01); // port C pin 1
			static const led_t Q1     = MXT_PINPORT(PORT_C, 0x02); // port C pin 2
			static const led_t Q2     = MXT_PINPORT(PORT_C, 0x04); // port C pin 3
			static const int CLED = 7;

		public:
			void turnOn(led_t led) { activate(led, true); }
			void turnOff(led_t led) { activate(led, false); }
			void activate(led_t, bool);
			void blink(led_t, const Time&);
		private:
			void doActivate(const void *);
			void blink_thread(led_t);
			static int get_led_id(led_t);

		private:
			qnx::Connection con_;
			Timer[CLED] blinkers_;
			bool[CLED] blinkState_;

		private:
			LED( );
			LED(const LED&);
			~LED( ) { }
			LED& operator=(const LED&);

			friend class Actuator;
			friend class SingletonInst;
	};

	typedef LED::SingletonInst LEDs;
}

#endif

