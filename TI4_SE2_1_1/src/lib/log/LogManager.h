#ifndef HAW_LIB_LOG_LOGMANAGER_H
#define HAW_LIB_LOG_LOGMANAGER_H

#include <map>

#include "lib/Singleton.hpp"
#include "lib/log/Logger.h"
#include "lib/concurrent/Lock.hpp"

namespace lib
{
	namespace log
	{
		/**
		 * LogManager Singleton, grants access to Logger instances.
		 * 
		 * Creates and exposes Logger instances by alphanumerical id.
		 *
		 * Offers a "root log" for convenience.
		 */
		class LogManagerImpl : public LockableObject<LogManagerImpl>
		{
			typedef LockableObject<LogManagerImpl> Super;
			typedef Super::Lock Lock;

			public:
				Logger_ptr rootLog( ) { return root_; }
				Logger_ptr getLog(const std::string&);

			private:
				Logger_ptr root_;
				std::map<std::string, Logger_ptr> logs_;

			public:
				LogManagerImpl( );
				~LogManagerImpl( ) { }
			private:
				LogManagerImpl(const LogManagerImpl&);
				LogManagerImpl& operator=(const LogManagerImpl&);
		};

		typedef Singleton
		<
			LogManagerImpl,
			SingletonConcurrency::MultiThreaded,
			CleanupUtility::DEFAULT_PRIORITY + 2
		>
		LogManager;
	}
}

#endif

