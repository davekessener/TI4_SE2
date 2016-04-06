#ifndef HAW_LIB_HWACCESS_H
#define HAW_LIB_HWACCESS_H

#include <stdint.h>

namespace lib
{
	class HWAccess
	{
		public:
			static HWAccess& instance( );

			uint16_t in(uint16_t) const;
			void out(uint16_t, uint16_t) const;
			void setBits(uint16_t, uint16_t) const;
			void resetBits(uint16_t, uint16_t) const;
		private:
			HWAccess( );
			~HWAccess( );
			HWAccess(const HWAccess&);
			HWAccess& operator=(const HWAccess&);
	};
}

#endif

