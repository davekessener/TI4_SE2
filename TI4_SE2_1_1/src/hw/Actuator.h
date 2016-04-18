#ifndef HAW_HW_ACTUATOR_H
#define HAW_HW_ACTUATOR_H

#include <Singleton.hpp>
#include <concurrent/Thread.h>
#include <qnx/Channel.h>

namespace hw
{
	class Actuator : public lib::LockableObject<Actuator>
	{
		typedef void (Actuator::*cmd_fn)(const void *);
		public:
			typedef lib::LockableObject<Actuator> Super;
			typedef lib::Singleton<Actuator, lib::SingletonConcurrency::MultiThreaded> SingletonInst;
			typedef Super::Lock Lock;

			using lib::qnx::Channel;
			using lib::Thread;

			static const uint8_t LED_ACTIVATE = 0x00;
			static const uint8_t MOTOR_BELT   = 0x01;
			static const uint8_t MOTOR_SWITCH = 0x02;
			static const int CCMD = 3;

		public:
			Channel getChannel( ) const { return ch_; }

		private:
			Channel ch_;
			Thread thread_;
			bool running_;
			cmd_fn[CCMD] cmds_;

			void thread( );
			void led_activate(const void *d) { LEDs::instance().doActivate(d); }
			void motor_belt(const void *d) { Motors::instance().doBelt(d); }
			void motor_switch(const void *d) { Motors::instance().doSwitch(d); }

		private:
			Actuator( );
			Actuator(const Actuator&);
			~Actuator( ) { }
			Actuator& operator=(const Actuator&);

			friend class SingletonInst;
	};

	typedef Actuator::SingletonInst Actuators;
}

#endif

