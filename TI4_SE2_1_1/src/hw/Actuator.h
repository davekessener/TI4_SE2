#ifndef HAW_HW_ACTUATOR_H
#define HAW_HW_ACTUATOR_H

#include "hw/LED.h"
#include "hw/Belt.h"
#include "hw/Switch.h"
#include "lib/Singleton.hpp"

namespace hw
{
	class Actuator
	{
		public:
			LED& getLeds( ) { static LED inst; return inst; }
			Belt& getBelt( ) { static Belt inst; return inst; }
			Switch& getSwitch( ) { static Switch inst; return inst; }
		private:
			Actuator( ) { }
			~Actuator( ) { }
			Actuator(const Actuator&);
			Actuator& operator=(const Actuator&);

			friend class lib::Singleton<Actuator, lib::SingletonConcurrency::Multithreaded>;
	};

	typedef lib::Singleton<Actuator, lib::SingletonConcurrency::Multithreaded> Actuators;
}

#endif

