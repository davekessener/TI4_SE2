#ifndef HAW_HW_SENSOR_H
#define HAW_HW_SENSOR_H

#include <memory>

#include "lib/concurrent/Lock.hpp"
#include "lib/Observer.hpp"
#include "lib/Singleton.hpp"

namespace hw
{
	class Sensor : public lib::LockableObject<Sensor>, public lib::Observable<sensor_t>
	{
		typedef lib::LockableObject<Sensor> Super;
		typedef Super::Lock Lock;

		public:
			typedef int sensor_t;
			static const sensor_t ENTERING = 0;
			static const sensor_t INHM = 1;
			static const sensor_t HMVALID = 2;
			static const sensor_t INSWITCH = 3;
			static const sensor_t ISMETAL = 4;
			static const sensor_t SWITCHOPEN = 5;
			static const sensor_t RAMPFULL = 6;
			static const sensor_t LEAVING = 7;
			static const sensor_t START = 8;
			static const sensor_t STOP = 9;
			static const sensor_t RESET = 10;
			static const sensor_t ESTOP = 11;

		public:
			bool get(sensor_t i) { return vals_[i]; }

		private:
			void manageHM( );
			uint16_t readHM( );

		private:
			static const int VAR_COUNT = 12;
			bool vals_[VAR_COUNT];
			bool running_;
			uint16_t height_;
			std::auto_ptr<lib::Thread> isr_thread_, hm_thread_;

		private:
			Sensor( );
			~Sensor( );
			Sensor(const Sensor&);
			Sensor& operator=(const Sensor&);

			friend class lib::Singleton<Sensor, lib::SingletonConcurrency::MultiThreaded>;
	};

	typedef lib::Singleton
	<
		Sensor,
		lib::SingletonConcurrency::MultiThreaded
	>
	Sensors;
}

#endif

