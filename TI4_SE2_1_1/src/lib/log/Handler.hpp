#ifndef HAW_LIB_LOG_HANDLER_H
#define HAW_LIB_LOG_HANDLER_H

#include <string>

#include "lib/log/Format.hpp"
#include "lib/mpl/types.hpp"
#include "lib/SmartPtr.hpp"

namespace lib
{
	namespace log
	{
		struct BaseHandler
		{
			public:
				BaseHandler(Formatter_ptr f) : f_(f) { }
				virtual ~BaseHandler( ) { }
				void handle(const LogRecord& lr) { handle(f_->format(lr)); }

			private:
				virtual void handle(const std::string&) = 0;
				
				Formatter_ptr f_;
		};

		template<typename F>
		class Handler : public BaseHandler
		{
			public:
				Handler(F f, Formatter_ptr p) : BaseHandler(p), f_(f) { }

			private:
				void handle(const std::string& s) { f_(s); }

				F f_;
		};

		typedef SmartPtr<BaseHandler> Handler_ptr;

		template<typename F>
		Handler_ptr toHandler(F f, Formatter_ptr p)
		{
			return Handler_ptr(new Handler<typename Decay<F>::Type>(f, p));
		}
	}
}

#endif
