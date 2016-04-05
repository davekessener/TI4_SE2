#ifndef HAW_LIB_HWACCESS_H
#define HAW_LIB_HWACCESS_H

#include <cstdint>

namespace lib
{
	class HWAccess
	{
		public:
			static HWAccess& instance( );

			std::uint16_t in(std::uint16_t) const;
			void out(std::uint16_t, std::uint16_t) const;
			void setBits(std::uint16_t, std::uint16_t) const;
			void resetBits(std::uint16_t, std::uint16_t) const;
		private:
			HWAccess( );
			~HWAccess( );
			HWAccess(const HWAccess&);
			HWAccess& operator=(const HWAccess&);
	};
}

#endif

