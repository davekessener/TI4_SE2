#ifndef HAW_RUN_READY_H
#define HAW_RUN_READY_H

#include "run/State.h"

namespace haw
{
	class Ready : public State
	{
		public:
			virtual void enter( );
			virtual void exit( );
//			virtual void update(lib::Time) { }
			virtual void process(const Event&);
//			virtual void execute( ) { }
		private:
	};
}

#endif

