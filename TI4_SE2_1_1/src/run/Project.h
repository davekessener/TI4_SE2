#ifndef HAW_RUN_PROJECT_H
#define HAW_RUN_PROJECT_H

#include <stdint.h>

#include "run/HM.h"
#include "run/State.h"
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
			~Project( );
			void run( );
			void calibrateHM(uint16_t min, uint16_t max) { hm_.calibrate(min, max); }
			void startHM( ) { hm_.start(); }
			uint32_t stopHM( ) { return hm_.stop(); }
			void calibrateDistances(lib::Time slow, lib::Time fast, lib::Time toHM, lib::Time puk);
			lib::Speed getSpeed(int s) const { return speeds_[s]; }
			uint32_t getPukWidth( ) const { return pukWidth_; }
			uint32_t hmPosition( ) const { return hmPos_; }
			uint32_t endPosition( ) const { return lib::Speed::reference; }
		private:
			void processEvents(State_ptr);
		private:
			HM hm_;
			lib::Speed speeds_[CSPEEDS];
			uint32_t pukWidth_, hmPos_;
	};
}

#endif

