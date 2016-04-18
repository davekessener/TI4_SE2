#ifndef HAW_HW_MOTOR_H
#define HAW_HW_MOTOR_H

#include <Singleton.hpp>
#include <qnx/Channel.h>
#include <concurrent/Lock.hpp>

namespace hw
{
	class Motor : public lib::LockableObject<Motor>
	{
		public:
			typedef lib::LockableObject<Motor> Super;
			typedef lib::Singleton<Motor, lib::SingletonConcurrency::MultiThreaded> SingletonInst;
			typedef Super::Lock Lock;
			typedef uint8_t pid_t;

			struct Direction
			{
				static const pid_t NONE  = 0x00; // no pin
				static const pid_t LEFT  = 0x01; // port A pin 0
				static const pid_t RIGHT = 0x02; // port A pin 1
			};

			struct Speed
			{
				static const pid_t FAST = 0x00; // no pin
				static const pid_t SLOW = 0x04; // port A pin 2
				static const pid_t STOP = 0x08; // port A pin 3
			};

			struct State
			{
				static const pid_t OPEN  = 0;
				static const pid_t CLOSE = 1;
			};

			static const pid_t SWITCH = 0x10; // port A pin 4

		public:
			void controlBelt(pid_t, pid_t);
			void controlSwitch(pid_t);

		private:
			void doControlBelt(pid_t, pid_t);
			void doControlSwitch(pid_t);
			
		private:
			pid_t curDir_;
			qnx::Connection con_;

		private:
			Motor( );
			Motor(const Motor&);
			~Motor( ) { }
			Motor& operator=(const Motor&);

			friend class Controller;
			friend class SingletonInst;
	};

	typedef Motor::SingletonInst Motors;
}

#endif

