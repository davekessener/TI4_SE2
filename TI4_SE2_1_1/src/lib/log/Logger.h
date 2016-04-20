#ifndef HAW_LIB_LOG_LOGGER_H
#define HAW_LIB_LOG_LOGGER_H

#include <vector>
#include <string>

#include "lib/macro.h"
#include "lib/log/Handler.hpp"
#include "lib/log/Filter.hpp"

#define MXT_LOGL(ll,msg,...) log(ll,msg,formatFILE(__FILE__),__LINE__,##__VA_ARGS__)
#define MXT_LOG(msg,...) MXT_LOGL(::lib::log::LogLevel::INFO,msg,##__VA_ARGS__)

namespace lib
{
	namespace log
	{
		class Logger : public LockableObject<Logger>
		{
			public:
			typedef SmartPtr<Logger> Logger_ptr;

			private:
			typedef LockableObject<Logger> Super;
			typedef Super::Lock Lock;
			typedef std::vector<Logger_ptr> parents_t;
			typedef	std::vector<Handler_ptr> handlers_t;
			typedef	std::vector<Filter_ptr> filters_t;
			typedef parents_t::iterator piter_t;
			typedef handlers_t::iterator hiter_t;
			typedef filters_t::iterator fiter_t;

			public:
				void addParent(Logger_ptr);
				void removeParent(Logger_ptr);
				void addHandler(Handler_ptr);
				void removeHandler(Handler_ptr);
				void addFilter(Filter_ptr);
				void removeFilter(Filter_ptr);
				void log(const LogLevel&, const std::string&, const char *, int, ...);
				void log(const LogRecord&);
			private:
				std::string name_;
				parents_t parents_;
				handlers_t handlers_;
				filters_t filters_;

			private:
				Logger( ) { }
				~Logger( ) { }
				Logger(const Logger&);
				Logger& operator=(const Logger&);

				friend class LogManagerImpl;
				friend class SmartPtr<Logger>;
		};

		typedef Logger::Logger_ptr Logger_ptr;
	}
}

#endif

