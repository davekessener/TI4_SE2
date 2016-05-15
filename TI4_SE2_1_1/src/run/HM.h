#ifndef HAW_RUN_HW_H
#define HAW_RUN_HW_H

#include <vector>

#include "hw/Sensor.h"
#include "run/Puk.h"

namespace haw
{
	class HM
	{
		public:
			HM( ) : avg_(0) { }
			void calibrate(uint16_t min, uint16_t max) { avg_ = (min + max) / 2; }
			void start( ) { tick(); }
			void tick( );
			uint32_t stop( );
			bool running( ) const { return !vals_.empty(); }
		private:
			uint16_t avg_;
			std::vector<uint16_t> vals_;
	};
}

#endif

