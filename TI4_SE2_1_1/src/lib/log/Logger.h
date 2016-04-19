#ifndef HAW_LIB_LOG_LOGGER_H
#define HAW_LIB_LOG_LOGGER_H

#include <vector>
#include <string>

#include "lib/macro.h"

#define MXT_LOG_1(msg) MXT_LOG_2(::lib::log::LogLevel::INFO,msg)
#define MXT_LOG_2(ll,msg) logger.log(ll, msg, formatFILE(__FILE__), __LINE__)
#define MXT_LOG(...) MXT_OVERLOAD(MXT_LOG_, __VA_ARGS__)

namespace lib
{
	namespace log
	{
		class Logger : public LockableObject<Logger>
		{
			typedef LockableObject<Logger> Super;
			typedef Super::Lock Lock;
			typedef std::vector<Logger *> parents_t;
			typedef	std::vector<Handler_ptr> handlers_t;
			typedef	std::vector<Filter_ptr> filters_t;
			typedef parents_t::iterator piter_t;
			typedef handlers_t::iterator hiter_t;
			typedef filters_t::iterator fiter_t;

			public:
				void addParent(Logger&);
				void removeParent(Logger&);
				void addHandler(Handler_ptr);
				void removeHandler(Handler_ptr);
				void addFilter(Filter_ptr);
				void removeFilter(Filter_ptr);
				void log(const LogLevel&, const std::string&, const char * = NULL, int = -1);
				void log(const LogRecord&);
			private:
				std::string name_;
				parents_t parents_;
				handlers_t handlers_;
				filters_t filters_;
		};
	}
}

#endif

