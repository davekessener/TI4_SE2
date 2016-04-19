#ifndef HAW_LIB_LOG_DEFAULTFORMAT_H
#define HAW_LIB_LOG_DEFAULTFORMAT_H

#include <string>
#include <stdint.h>

#include "lib/log/LogRecord.h"
#include "lib/concurrent/Lock.hpp"

namespace lib
{
	namespace log
	{
		class LogRecord;

		class DefaultFormatter : public LockableObject<DefaultFormatter>
		{
			typedef LockableObject<DefaultFormatter> Super;
			typedef Super::Lock Lock;

			public:
				std::string operator()(const LogRecord&);
				static std::string toDate(uint64_t);
			private:
		};
	}
}

#endif

