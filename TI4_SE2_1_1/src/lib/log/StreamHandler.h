#ifndef HAW_LIB_LOG_STREAMHANDLER_H
#define HAW_LIB_LOG_STREAMHANDLER_H

#include <iosfwd>
#include <string>

#include "lib/concurrent/Lock.hpp"

namespace lib
{
	namespace log
	{
		class StreamHandler : public LockableObject<StreamHandler>
		{
			typedef LockableObject<StreamHandler> Super;
			typedef Super::Lock Lock;

			public:
				StreamHandler(std::ostream *os) : os_(os) { }
				void operator()(const std::string&);
			private:
				std::ostream *os_;
		};
	}
}

#endif

