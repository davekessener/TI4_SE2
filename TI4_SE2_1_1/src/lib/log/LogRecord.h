#ifndef HAW_LIB_LOG_LOGRECORD_H
#define HAW_LIB_LOG_LOGRECORD_H

#include <string>
#include <stdint.h>

#include "lib/log/LogLevel.h"

namespace lib
{
	namespace log
	{
		class LogRecord
		{
			public:
				LogRecord(LogLevel ll, uint64_t ts, const std::string& msg, const char *f = NULL, int l = -1)
					: ll_(ll), ts_(ts), msg_(msg), f_(f), l_(l) { }
				const LogLevel& logLevel( ) const { return ll_; }
				uint64_t timestamp( ) const { return ts_; }
				const std::string& message( ) const { return msg_; }
				const char *file( ) const { return f_; }
				int line( ) const { return l_; }
				bool hasFile( ) const { return f_; }
			private:
				LogLevel ll_;
				uint64_t ts_;
				std::string msg_;
				const char *f_;
				int l_;
		};
	}
}

#endif

