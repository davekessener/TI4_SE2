#ifndef HAW_RUN_RUNNING_H
#define HAW_RUN_RUNNING_H

#include <vector>

#include "run/Project.h"
#include "run/State.h"
#include "run/Puk.h"

namespace haw
{
	class Running : public State
	{
		typedef std::vector<Puk> vec_t;
		typedef vec_t::iterator iter_t;

		static const uint32_t PUK_FLAT = 0;
		static const uint32_t PUK_LARGE = 1;
		static const uint32_t PUK_METAL = 2;
		static const uint32_t CPUKS = 3;

		public:
			Running( );
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
			inline int getSpeed(int s) { return s == Project::SPEED_SLOW ? Motor::Speed::SLOW : (s == Project::SPEED_FAST ? Motor::Speed::FAST : Motor::Speed::STOP); }
			inline uint32_t toType(Puk& p) { return p.metal() ? PUK_METAL : (p.type() == Puk::Type::FLAT ? PUK_FLAT : PUK_LARGE); }
		private:
			Project &project_;
			vec_t puks_;
			Puk *hmPuk_;
			int32_t speed_, toSwitchClose_, scKeep_, scKick_;
			uint32_t nextPuk_;
			bool stopping_, pausing_;
	};
}

#endif

