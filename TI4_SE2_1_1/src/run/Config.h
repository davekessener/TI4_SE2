#ifndef HAW_RUN_CONFIG_H
#define HAW_RUN_CONFIG_H

#include <string>
#include <stdint.h>

#include "lib/TimeP.h"

namespace haw
{
	class Config
	{
		public:
			Config(const std::string&);
			void save( ) const;
			void setHM(uint16_t min, uint16_t max) { min_ = min; max_ = max; }
			void setTimes(lib::Time slow, lib::Time fast, lib::Time hm, lib::Time puk) { slow_ = slow; fast_ = fast; toHM_ = hm; puk_ = puk; }
			uint16_t getMin( ) const { return min_; }
			uint16_t getMax( ) const { return max_; }
			lib::Time getSlow( ) const { return slow_; }
			lib::Time getFast( ) const { return fast_; }
			lib::Time getHM( ) const { return toHM_; }
			lib::Time getPuk( ) const { return puk_; }
		private:
			std::string path_;
			uint16_t min_, max_;
			lib::Time slow_, fast_, toHM_, puk_;
	};
}

#endif
