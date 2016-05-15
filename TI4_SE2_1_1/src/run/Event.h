#ifndef HAW_RUN_EVENT_H
#define HAW_RUN_EVENT_H

namespace haw
{
	class Event
	{
		public:
		typedef event_id_t;
		struct ID
		{
			static const event_id_t SENSOR = 1;
		};

		public:
			virtual ~Event( ) { }
			virtual event_id_t id( ) const = 0;
	};

	class SensorEvent
	{
		public:
		struct Sensors
		{
			static const uint32_t ENTERING = 0;
			static const uint32_t LEAVING = 1;
			static const uint32_t IN_HM = 2;
			static const uint32_t IN_SWITCH = 3;
			static const uint32_t IS_METAL = 4;
			static const uint32_t RAMP_FULL = 5;
			static const uint32_t START = 6;
			static const uint32_t STOP = 7;
			static const uint32_t RESET = 8;
			static const uint32_t ESTOP = 9;
		};
		typedef uint32_t sensor_t;

		public:
			SensorEvent(sensor_t s, bool v) : s_(s), v_(v) { }
			virtual event_id_t id( ) const { return Event::ID::SENSOR; }
			sensor_t sensor( ) const { return s; }
			bool value( ) const { return v_; }
		private:
			sensor_t s_;
			bool v_;
	};
}

#endif

