#ifndef HAW_HW_ACTUATOR_H
#define HAW_HW_ACTUATOR_H

#include <memory>

#include <lib/Singleton.hpp>
#include <lib/concurrent/Thread.h>
#include <lib/qnx/Channel.h>
#include <hw/LED.h>
#include <hw/Motor.h>

namespace hw
{
/**
 * Singular access point to actuators.
 * The Actuator class encapsulates access to
 * all actuators of the attached hw unit,
 * including LEDs, the conveyor belt and the
 * electromagnetic switch.
 * The dispatcher runs in its own thread and
 * communicates via lib::qnx::Channel.
 * It is a singleton via lib::Singleton template.
*/
	class Actuator : public lib::LockableObject<Actuator>
	{
		typedef void (Actuator::*cmd_fn)(const void *);
		public:
			typedef lib::LockableObject<Actuator> Super;
			typedef lib::Singleton<Actuator, lib::SingletonConcurrency::MultiThreaded> SingletonInst;
			typedef Super::Lock Lock;

			typedef lib::qnx::Channel Channel;
			typedef lib::Thread Thread;

			static const uint8_t LED_ACTIVATE = 0x00;
			static const uint8_t MOTOR_BELT   = 0x01;
			static const uint8_t MOTOR_SWITCH = 0x02;
			static const int CCMD = 3;

		public:
			const Channel& getChannel( ) const { return ch_; }

		private:
			Channel ch_;
			bool running_;
			cmd_fn cmds_[CCMD];
			std::auto_ptr<Thread> thread_;

			void thread( );
			void led_activate(const void *d) { LEDs::instance().doActivate(d); }
			void motor_belt(const void *d) { Motors::instance().doControlBelt(d); }
			void motor_switch(const void *d) { Motors::instance().doControlSwitch(d); }

		public:
			Actuator( );
			~Actuator( ) { running_ = false; ch_.close(); thread_->join(); }
		private:
			Actuator(const Actuator&);
			Actuator& operator=(const Actuator&);
	};

	typedef Actuator::SingletonInst Actuators;
}

#endif

