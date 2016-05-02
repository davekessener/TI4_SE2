#ifndef HAW_HW_SENSOR_H
#define HAW_HW_SENSOR_H

#include "lib/qnx/Channel.h"

namespace hw
{
	class Sensor
	{
		public:
			bool entering( ) { cEntering_ = false; return entering_ };
			bool enteringChanged( ) const { return cEntering_; };
			bool inHM( ) { cInHm_ = false; return inHM_; }
			bool inHMChanged( ) const { return cInHM_; }
			bool hmValid( ) { cHmValid_ = false; return hmValid_; }
			bool hmValieChanged( ) const { return cHmValid_; }
			bool inSwitch( ) { cInSwitch_ = false; return inSwitch_; }
			bool inSwitchChanged( ) const { return cInSwitch_;
			bool isMetal( ) { cIsMetal_ = false; return isMetal_; }
			bool isMetalChanged( ) const { return cIsMetal_; }
			bool switchOpen( ) { cSwitchOpen_ = false; return switchOpen_; }
			bool switchOpenChanged( ) const { return cSwitchOpen_; }
			bool rampFull( ) { cRampFull_ = false; return rampFull_; }
			bool rampFullChanged( ) const { return cRampFull_; }
			bool leaving( ) { cLeaving_ = false; return leaving_; }
			bool leavingChanged( ) const { return cLeaving_; }

		private:
			void thread( );

		private:
			bool entering_, inHM_, hmValid_, inSwitch_, isMetal_, switchOpen_, rampFull_, leaving_;
			bool cEntering_, cInHM_, cHmValid_, cInSwitch_, cIsMetal_, cSwitchOpen_, cRampFull_, cLeaving_;
			bool running_;
			lib::qnx::Channel ch_;
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

