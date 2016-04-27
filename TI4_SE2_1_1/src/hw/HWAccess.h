#ifndef HAW_HW_HWACCESS_H
#define HAW_HW_HWACCESS_H

#include <stdint.h>
#include <lib/concurrent/Lock.hpp>
#include <lib/Singleton.hpp>

namespace hw
{
/**
 * Interface for direct hardware access.
 * The HWAccess singleton offers read/write operations
 * to the three ports of the hw unit.
*/
	class HWAccessImpl : public lib::LockableClass<HWAccessImpl>
	{
		public:
			typedef lib::LockableClass<HWAccessImpl> Super;
			typedef lib::Singleton<HWAccessImpl, lib::SingletonConcurrency::MultiThreaded> SingletonInst;

		public:
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

		public:
			HWAccessImpl( );
			~HWAccessImpl( ) { }
		private:
			HWAccessImpl(const HWAccessImpl&);
			HWAccessImpl& operator=(const HWAccessImpl&);
	};

	typedef HWAccessImpl::SingletonInst HWAccess;
}

#endif

