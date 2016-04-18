#ifndef HAW_LIB_TIME_H
#define HAW_LIB_TIME_H

#include <stdint.h>
#include <time.h>

namespace lib
{
	class Time
	{
		public:
		typedef uint32_t us_t;
		static const uint32_t MS_TO_US = 1000;
		static const uint32_t  S_TO_MS = 1000;
		static const uint32_t  M_TO_S  = 60;
		static const uint32_t  H_TO_M  = 60;
		static const uint32_t  S_TO_US = S_TO_MS * MS_TO_US;
		static const uint32_t  M_TO_US = M_TO_S  * S_TO_US;
		static const uint32_t  H_TO_US = H_TO_M  * M_TO_US;

		public:
			explicit Time(us_t) : t_(us_t) { }
			void wait( ) const { sleep(t_); }
			void toTimespec(struct timespec *);
			us_t raw( ) const { return t_; }

			static Time h(us_t v) { return Time(v * H_TO_US); }
			static Time min(us_t v) { return Time(v * M_TO_US); }
			static Time s(us_t v) { return Time(v * S_TO_US); }
			static Time ms(us_t v) { return Time(v * MS_TO_US); }
			static Time us(us_t v) { return Time(v); }
			static void sleep(us_t);
		private:
			us_t t_;
	};

	class Frequency
	{
		public:
			static Time Hz(double v) { return Time::us(static_cast<Time::us_t>(1000000.0 / v)); }
			static Time kHz(double v) { return Time::us(static_cast<Time::us_t>(1000.0 / v)); }
			static Time MHz(double v) { return Time::us(static_cast<Time::us_t>(1.0 / v)); }
	};
}

#endif

