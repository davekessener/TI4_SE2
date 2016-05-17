#ifndef HAW_RUN_RUNNING_H
#define HAW_RUN_RUNNING_H

#include <vector>

#include "run/Project.h"
#include "run/State.h"
#include "run/Puk.h"
#include "hw/Motor.h"

namespace haw
{
	class Running : public State
	{
		typedef std::vector<Puk_ptr> vec_t;
		typedef vec_t::iterator iter_t;

		public:
		static const uint32_t PUK_FLAT = 0;
		static const uint32_t PUK_LARGE = 1;
		static const uint32_t PUK_METAL = 2;
		static const uint32_t CPUKS = 3;

		public:
			Running(Project&);
			virtual void enter( );
			virtual void exit( );
			virtual void update(lib::Time);
  			virtual void process(const Event&);
			virtual void execute( );
		private:
			void enterHM( );
			void exitHM( );
			void enterLeaving( );
			void exitLeaving( );
			void handleSwitch( );
			bool keepPuk(Puk&);
			iter_t anyPukIn(uint32_t);
			inline bool switchClosed( ) const { return toSwitchClose_ <= 0; }
		private:
			Project &project_;
			vec_t puks_;
			Puk_ptr hmPuk_;
			int32_t speed_, toSwitchClose_, scKeep_, scKick_;
			uint32_t nextPuk_;
			bool stopping_, pausing_;
	};
}

#endif

