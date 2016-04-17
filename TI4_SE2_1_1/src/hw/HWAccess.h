#ifndef HAW_HW_HWACCESS_H
#define HAW_HW_HWACCESS_H

#include <stdint.h>
#include <concurrent/Lock.hpp>

namespace hw
{
	class HWAccess : public lib::LockableClass<HWAccess>
	{
		typedef lib::LockableClass<HWAccess> Super;

		public:
			static HWAccess& instance( );

			typedef uint16_t port_t;
			typedef uint8_t pin_t;

			static const port_t PORT_A = 0x300;
			static const port_t PORT_B = 0x301;
			static const port_t PORT_C = 0x302;
			
			uint8_t in(port_t) const;
			void out(port_t, pin_t) const;
			void setBits(port_t, pin_t) const;
			void resetBits(port_t, pin_t) const;

			struct Lock : public Super::Lock { Lock( ) : Super::Lock(NULL) { } };
		private:
			HWAccess( );
			~HWAccess( );
			HWAccess(const HWAccess&);
			HWAccess& operator=(const HWAccess&);
	};
}

#endif

