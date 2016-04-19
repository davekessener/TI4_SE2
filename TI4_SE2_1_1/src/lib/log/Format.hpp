#ifndef HAW_LIB_LOG_FORMAT_H
#define HAW_LIB_LOG_FORMAT_H

#include <string>

#include "lib/log/LogRecord.h"
#include "lib/mpl/types.hpp"
#include "lib/SmartPtr.hpp"

namespace lib
{
	namespace log
	{
		struct BaseFormatter
		{
			virtual ~BaseFormatter( ) { }
			virtual std::string format(const LogRecord&) = 0;
		};

		template<typename F>
		struct Formatter : public BaseFormatter
		{
			Formatter(F f) : f_(f) { }
			std::string format(const LogRecord& lr) { return f_(lr); }
			private: F f_;
		};

		typedef SmartPtr<BaseFormatter> Formatter_ptr;

		template<typename F>
		Formatter_ptr toFormatter(F f)
		{
			return Formatter_ptr(new Formatter<typename Decay<F>::Type>(f));
		}
	}
}

#endif

