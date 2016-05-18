#ifndef HAW_HW_MOTOR_H
#define HAW_HW_MOTOR_H

#include <lib/Singleton.hpp>
#include <lib/qnx/Channel.h>
#include <lib/concurrent/Lock.hpp>

namespace hw
{
	class Actuator;

/*! \class Motor
 * \brief Interface for controlling the motors.
 * Client interface for controlling the conveyor belt and electromagnetic switch.
 * Defines different states that the motors can enter.
 */
	class Motor : public lib::LockableObject<Motor>
	{
		public:
			typedef lib::LockableObject<Motor> Super;
			typedef lib::Singleton<Motor, lib::SingletonConcurrency::MultiThreaded> SingletonInst;
			typedef Super::Lock Lock;
			typedef uint8_t pid_t;

			struct Direction
			{
				static const pid_t NONE  = 0x00; //!< no pin
				static const pid_t RIGHT = 0x01; //!< port A pin 0
				static const pid_t LEFT  = 0x02; //!< port A pin 1
			};

			struct Speed
			{
				static const pid_t FAST = 0x00; //!< no pin
				static const pid_t SLOW = 0x04; //!< port A pin 2
				static const pid_t STOP = 0x08; //!< port A pin 3
			};

			struct State
			{
				static const pid_t OPEN  = 0;
				static const pid_t CLOSE = 1;
			};

			static const pid_t SWITCH = 0x10; //!< port A pin 4

		public:
			/** Controls conveyor belt.
			 * @param dir <tt>Direction</tt> the conveyor belt is supposed to move in.
			 * @param speed <tt>Speed</tt> of the conveyor belt.
			 *
			 * If <tt>dir == Direction::NONE</tt> <i>or</i> <tt>speed == Speed::STOP</tt>
			 * the conveyor belt is turned off, but never suppressed.
			 */
			void controlBelt(pid_t dir, pid_t speed);
			/** Controls electromagnetic switch.
			 * @param state <tt>State</tt> the switch is supposed to be in.
			 */
			void controlSwitch(pid_t state);

		private:
			void doControlBelt(const void *);
			void doControlSwitch(const void *);
			
		private:
			pid_t curDir_;
			lib::qnx::Connection con_;

		public:
			Motor( );
			~Motor( ) { }
		private:
			Motor(const Motor&);
			Motor& operator=(const Motor&);

			friend class Actuator;
	};

	typedef Motor::SingletonInst Motors;
}

#endif

