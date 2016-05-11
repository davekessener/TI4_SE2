#ifndef HAW_HW_SENSOR_H
#define HAW_HW_SENSOR_H

#include <memory>

#include "lib/concurrent/Thread.h"
#include "lib/qnx/Channel.h"
#include "lib/Singleton.hpp"

namespace hw
{
	class Sensor
	{
		static const int ENTERING = 0;
		static const int INHM = 1;
		static const int HMVALID = 2;
		static const int INSWITCH = 3;
		static const int ISMETAL = 4;
		static const int SWITCHOPEN = 5;
		static const int RAMPFULL = 6;
		static const int LEAVING = 7;

		public:
			bool entering( ) { return get(ENTERING); }
			bool enteringChanged( ) const { return flags_[ENTERING]; }
			bool inHM( ) { return get(INHM); }
			bool inHMChanged( ) const { return flags_[INHM]; }
			bool hmValid( ) { return get(HMVALID); }
			bool hmValieChanged( ) const { return flags_[HMVALID]; }
			bool inSwitch( ) { return get(INSWITCH); }
			bool inSwitchChanged( ) const { return flags_[INSWITCH]; }
			bool isMetal( ) { return get(ISMETAL); }
			bool isMetalChanged( ) const { return flags_[ISMETAL]; }
			bool switchOpen( ) { return get(SWITCHOPEN); }
			bool switchOpenChanged( ) const { return flags_[SWITCHOPEN]; }
			bool rampFull( ) { return get(RAMPFULL); }
			bool rampFullChanged( ) const { return flags_[RAMPFULL]; }
			bool leaving( ) { return get(LEAVING); }
			bool leavingChanged( ) const { return flags_[LEAVING]; }

		private:
			void thread( );
			void handlePulse(uint32_t);
			void initISR( );
			void cleanupISR( );
			bool get(int i) { flags_[i] = false; return vals_[i]; }

		private:
			static const int VAR_COUNT = 8;
			bool vals_[VAR_COUNT], flags_[VAR_COUNT];
			bool running_;
			lib::qnx::Channel ch_;
			lib::qnx::Connection con_;
			std::auto_ptr<lib::Thread> thread_;

		public:
			Sensor( );
			~Sensor( );
		private:
			Sensor(const Sensor&);
			Sensor& operator=(const Sensor&);
	};

	typedef lib::Singleton
	<
		Sensor,
		lib::SingletonConcurrency::MultiThreaded
	>
	Sensors;
}

#endif

