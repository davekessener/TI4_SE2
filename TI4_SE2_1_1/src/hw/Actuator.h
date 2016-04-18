#ifndef HAW_HW_ACTUATOR_H
#define HAW_HW_ACTUATOR_H

#include <Singleton.hpp>
#include <concurrent/Thread.h>
#include <qnx/Channel.h>

namespace hw
{
	class Actuator : public lib::LockableObject<Actuator>
	{
		public:
			typedef lib::LockableObject<Actuator> Super;
			typedef lib::Singleton<Actuator, lib::SingletonConcurrency::MultiThreaded> SingletonInst;
			typedef Super::Lock Lock;

			using lib::qnx::Channel;
			using lib::Thread;

		public:
			Channel getChannel( ) const { return ch_; }

		private:
			Channel ch_;
			Thread thread_;
			bool running_;

			void thread( );

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

