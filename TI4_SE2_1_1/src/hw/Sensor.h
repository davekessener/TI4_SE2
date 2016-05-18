#ifndef HAW_HW_SENSOR_H
#define HAW_HW_SENSOR_H

#include <memory>

#include "lib/concurrent/Thread.h"
#include "lib/concurrent/Lock.hpp"
#include "lib/qnx/Channel.h"
#include "lib/Singleton.hpp"

namespace hw
{
	
/*! \class Sensor
 * \brief Interface for checking the state of the Sensors.
 * The sensors can be checked for different states. 
 * For example if the sensor is still active
 * or the State of the Sensor changed.
 * 
 * Additionally the current height of the HM can be read.
 */
	class Sensor : public lib::LockableObject<Sensor>
	{
		typedef lib::LockableObject<Sensor> Super;
		typedef Super::Lock Lock;

/*! \brief Sensor ISRs
 *        
 * Defines for the different Sensors.
 * States and Flags are saved in vals_[] and flags_[].
 */
		static const int ENTERING = 0;
		static const int INHM = 1;
		static const int HMVALID = 2;
		static const int INSWITCH = 3;
		static const int ISMETAL = 4;
		static const int SWITCHOPEN = 5;
		static const int RAMPFULL = 6;
		static const int LEAVING = 7;
		static const int START = 8;
		static const int STOP = 9;
		static const int RESET = 10;
		static const int ESTOP = 11;

/*! \brief Public functions to check the state of the Sensors
 *        
 * Functions containing Changed in the Title check the flags, if the state of the Sensor changed.
 * Functions without Changed in the Title check the current State of the Sensor.
 */
		public:
			bool entering( ) { return get(ENTERING); }
			bool enteringChanged( ) const { return flags_[ENTERING]; }
			bool inHM( ) { return get(INHM); }
			bool inHMChanged( ) const { return flags_[INHM]; }
			bool hmValid( ) { return get(HMVALID); }
			bool hmValidChanged( ) const { return flags_[HMVALID]; }
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
			bool start( ) { return get(START); }
			bool startChanged( ) const { return flags_[START]; }
			bool stop( ) { return get(STOP); }
			bool stopChanged( ) const { return flags_[STOP]; }
			bool reset( ) { return get(RESET); }
			bool resetChanged( ) const { return flags_[RESET]; }
			bool estop( ) { return get(ESTOP); }
			bool estopChanged( ) const { return flags_[ESTOP]; }
			uint16_t getHeight( ) const { return height_; }

			void handlePulse(uint32_t);
			void shutdown( );

		private:
			bool get(int i) { Lock guard(this); flags_[i] = false; return vals_[i]; }
			void manageHM( );

		private:
			static const int VAR_COUNT = 12; //!< Current Number of Sensors checked through ISRs.
			bool vals_[VAR_COUNT], flags_[VAR_COUNT]; //!< Arrays for saving States and Flags of the Sensors.
			bool running_;
			uint16_t height_; //!< Holds the current height of the Object moving through the HM.
			std::auto_ptr<lib::Thread> isr_thread_, hm_thread_;

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

