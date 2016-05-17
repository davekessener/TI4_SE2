#ifndef HAW_RUN_PROJECT_H
#define HAW_RUN_PROJECT_H

#include <stdint.h>

#include "run/HM.h"
#include "run/State.h"
#include "run/Config.h"
#include "lib/TimeP.h"

namespace haw
{
	class Project
	{
		public:
		static const int SPEED_STOP = 0;
		static const int SPEED_SLOW = 1;
		static const int SPEED_FAST = 2;
		static const int CSPEEDS = 3;

		public:
			Project( );
			~Project( );
			void run( );
			void calibrateHM(uint16_t, uint16_t);
			void startHM( ) { hm_.start(); }
			uint32_t stopHM( ) { return hm_.stop(); }
			void calibrateDistances(lib::Time slow, lib::Time fast, lib::Time toHM, lib::Time puk);
			void calibrateFromConfig( );
			void saveConfig( );
			lib::Speed getSpeed(int s) const { return speeds_[s]; }
			uint32_t getPukWidth( ) const { return pukWidth_; }
			uint32_t hmPosition( ) const { return hmPos_; }
			uint32_t endPosition( ) const { return lib::Speed::reference - pukWidth_*3/2; }
		private:
			void processEvents(State_ptr);
		private:
			HM hm_;
			Config config_;
			lib::Speed speeds_[CSPEEDS];
			uint32_t pukWidth_, hmPos_;
	};
}

#endif

