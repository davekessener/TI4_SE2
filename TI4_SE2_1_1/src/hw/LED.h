#ifndef HAW_HW_LED_H
#define HAW_HW_LED_H

#include <Time.h>

namespace hw
{
	class Controller;

	class LED
	{
		public:
			using HWAccess::PORT_A, HWAccess::PORT_C;

			typedef uint32_t led_t;
			static const led_t GREEN  = MXT_PINPORT(PORT_A, 0x20); // port A pin 5
			static const led_t YELLOW = MXT_PINPORT(PORT_A, 0x40); // port A pin 6
			static const led_t RED    = MXT_PINPORT(PORT_A, 0x80); // port A pin 7
			static const led_t START  = MXT_PINPORT(PORT_C, 0x00); // port C pin 0
			static const led_t RESET  = MXT_PINPORT(PORT_C, 0x01); // port C pin 1
			static const led_t Q1     = MXT_PINPORT(PORT_C, 0x02); // port C pin 2
			static const led_t Q2     = MXT_PINPORT(PORT_C, 0x04); // port C pin 3

		public:
			static void turnOn(led_t);
			static void turnOff(led_t);
			static void blink(led_t, const Time&);
		private:
			static void doTurnOn(led_t);
			static void doTurnOff(led_t);

			friend class Controller;
	};
}

#endif

