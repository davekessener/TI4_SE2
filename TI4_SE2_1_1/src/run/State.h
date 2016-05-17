#ifndef HAW_RUN_STATE_H
#define HAW_RUN_STATE_H

#include <stdint.h>

#include "lib/SmartPtr.hpp"
#include "lib/TimeP.h"
#include "run/Event.h"

namespace haw
{
	class State
	{
		public:
		typedef int32_t update_t;

		static const update_t PREVIOUS = -1;
		static const update_t THIS = 0;
		static const update_t NEXT = 1;

		public:
			State( ) : next_(THIS) { }
			virtual ~State( ) { }
			virtual void enter( ) { }
			virtual void exit( ) { }
			virtual void update(lib::Time) { }
			virtual void process(const Event&) { }
			virtual void execute( ) { }
			virtual update_t getNext( ) { return next_; };
		protected:
			void setNext(update_t u) { next_ = u; }
		private:
			update_t next_;
	};

	typedef lib::SmartPtr<State> State_ptr;
}

#endif

