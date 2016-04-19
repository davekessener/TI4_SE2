#ifndef HAW_LIB_LOG_LOGMANAGER_H
#define HAW_LIB_LOG_LOGMANAGER_H

#include "lib/Singleton.hpp"
#include "lib/log/Logger.h"

namespace lib
{
	namespace log
	{
		class LogManagerImpl
		{
			public:
				LogManagerImpl( ) { }
				~LogManagerImpl( ) { }
			private:
				LogManagerImpl(const LogManagerImpl&);
				LogManagerImpl& operator=(const LogManagerImpl&);
		};

		typedef Singleton
		<
			LogManagerImpl,
			SingletonConcurrency::MultiThreaded,
			CleanupUtility::DEFAULT_PRIORITY + 1
		>
		LogManager;
	}
}

#endif

