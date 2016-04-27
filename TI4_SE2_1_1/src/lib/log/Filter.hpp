#ifndef HAW_LIB_LOG_FILTER_H
#define HAW_LIB_LOG_FILTER_H

#include "lib/mpl/types.hpp"
#include "lib/log/LogRecord.h"
#include "lib/SmartPtr.hpp"

namespace lib
{
	namespace log
	{
		struct BaseFilter
		{
			virtual ~BaseFilter( ) { }
			virtual bool accept(const LogRecord&) = 0;
		};

		/**
		 * Filter template that accepts functors.
		 *
		 * Any logged LogRecord is passed through all filters
		 * of the give Logger instance. If any reject it, it
		 * will be discarded.
		 */
		template<typename F>
		struct Filter : public BaseFilter
		{
			Filter(F f) : f_(f) { }
			bool accept(const LogRecord& lr) { return f_(lr); }
			private: F f_;
		};

		typedef SmartPtr<BaseFilter> Filter_ptr;

		template<typename F>
		Filter_ptr toFilter(F f)
		{
			return Filter_ptr(new Filter<typename Decay<F>::Type>(f));
		}
	}
}

#endif

