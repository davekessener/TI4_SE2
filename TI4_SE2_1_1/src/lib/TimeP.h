#ifndef HAW_LIB_TIME_H
#define HAW_LIB_TIME_H

#include <stdint.h>
#include <time.h>

namespace lib
{
/** Data class representing a timeframe with microsecond accuracy.
 * Allows suspension of current thread via <tt>sleep</tt>.
 * Convenient for intentional delays:
 * <tt>Time::ms(500).wait()</tt> suspends the currently active
 * thread for 500ms.
 */
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
			Time( ) : t_(0) { }
			explicit Time(us_t t) : t_(t) { }
			void wait( ) const { sleep(t_); }
			void toTimespec(timespec *);
			us_t raw( ) const { return t_; }
			Time& operator+=(const Time& t) { t_ += t.t_; return *this; }
			Time operator+(const Time& t) const { Time tt(*this); return tt += t; }

			static Time h(us_t v) { return Time(v * H_TO_US); }
			static Time min(us_t v) { return Time(v * M_TO_US); }
			static Time s(us_t v) { return Time(v * S_TO_US); }
			static Time ms(us_t v) { return Time(v * MS_TO_US); }
			static Time us(us_t v) { return Time(v); }
			static void sleep(us_t);
		private:
			us_t t_;
	};

/** Convenience class that allows calculation of a signal's period length through its frequency.
 */
	class Frequency
	{
		public:
			static Time Hz(double v) { return Time::us(static_cast<Time::us_t>(1000000.0 / v)); }
			static Time kHz(double v) { return Time::us(static_cast<Time::us_t>(1000.0 / v)); }
			static Time MHz(double v) { return Time::us(static_cast<Time::us_t>(1.0 / v)); }
		private:
			Frequency( );
			~Frequency( );
			Frequency(const Frequency&);
			Frequency& operator=(const Frequency&);
	};

	class Speed
	{
		public:
			static const uint32_t reference = 1 << 26;

		public:
			Speed( ) : v_(0) { }
			Speed(Time t) : v_(t.raw() ? reference / t.raw() : 0) { }
			uint32_t in(Time t) const { return v_ * t.raw(); }
		private:
			uint32_t v_;
	};
}

#endif

