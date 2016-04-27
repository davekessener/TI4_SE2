#ifndef HAW_LIB_CLEANUPUTILITY_H
#define HAW_LIB_CLEANUPUTILITY_H

#include <vector>
#include <queue>

#include <lib/concurrent/Lock.hpp>

namespace lib
{
/**
 * Utility for controlling the lifetime of static objects, i.e. \ Singletons.
 * This utility class offers a more fine-grained, priority based version of
 * clib's lifo based ::atexit(void (*)(void)) function.
 * Functors are executed highest (numerically smallest) priority first.
 */
	class CleanupUtility : public LockableObject<CleanupUtility>
	{
		typedef LockableObject<CleanupUtility> Super;
		typedef Super::Lock Lock;
		typedef void (*atexit_fn)(void);
		typedef std::pair<size_t, atexit_fn> value_type;

		struct Compare { bool operator()(const value_type& a, const value_type& b) { return a.first <= b.first; } };

		typedef std::priority_queue<value_type, std::vector<value_type>, Compare> queue_t;

		public:
			static CleanupUtility& instance( );

			static const size_t DEFAULT_PRIORITY = 10;

			void scheduleAtExit(atexit_fn f) { scheduleAtExitWithPriority(f, DEFAULT_PRIORITY); }
			void scheduleAtExitWithPriority(atexit_fn, size_t);

		private:
			queue_t queue_;

		private:
			CleanupUtility( ) { }
			CleanupUtility(const CleanupUtility&);
			~CleanupUtility( );
			CleanupUtility& operator=(const CleanupUtility&);
	};
}

#endif

