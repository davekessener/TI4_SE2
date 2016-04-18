#include <CleanupUtility.h>

namespace lib
{

CleanupUtility::~CleanupUtility(void)
{
	Lock guard(this);

	try
	{
		while(!queue_.empty())
		{
			atexit_fn f = queue_.top().second;

			queue_.pop();

			(*f)();
		}
	}
	catch(...)
	{
		std::terminate();
	}
}

void CleanupUtility::scheduleAtExitWithPriority(atexit_fn f, size_t p)
{
	Lock guard(this);

	queue_.push(std::make_pair(f, p));
}

CleanupUtility& CleanupUtility::instance(void)
{
	static CleanupUtility inst;
	return inst;
}

}

