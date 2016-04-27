#include <pthread.h>

#include "lib/Timer.h"

namespace lib
{

class TimerPoolImpl
{
	public:
		typedef Singleton<TimerPoolImpl> SingletonInst;

	private:
		void addTimer(Timer *t) { timers_.push_back(t); }
		void removeTimer(Timer *t) { timers_.erase(std::find(timers_.begin(), timers_.end(), t)); }
		void deactivateAll( );

	private:
		std::vector<Timer *> timers_;

	public:
		TimerPoolImpl( ) { }
		~TimerPoolImpl( ) { deactivateAll(); }
	private:
		TimerPoolImpl(const TimerPoolImpl&);
		TimerPoolImpl& operator=(const TimerPoolImpl&);

		friend class Timer;
};

typedef TimerPoolImpl::SingletonInst TimerPool;

// # ===========================================================================

bool Timer::gactive = true;

Timer::Timer(void) : t_(timestamp())
{
	TimerPool::instance().addTimer(this);
}

Timer::~Timer(void)
{
	TimerPool::instance().removeTimer(this);
	deactivate();
}

void Timer::sync(Time t)
{
	
}

void Timer::reset(void)
{
	lapse();
}

Time Timer::delta(void)
{
	return Time::us(lapse() / 1000);
}

Time Timer::elapsed(void)
{
	return Time::us(get() / 1000);
}

bool Timer::active(void) const
{
	return thread_.get();
}

void Timer::deactivate(void)
{
	if(active())
	{
		cond_.lock();

		std::auto_ptr<Thread> p(thread_);

		cond_.broadcast();
		cond_.unlock();

		p->join();
	}
}

// # ---------------------------------------------------------------------------

void Timer::deactivateAll(void)
{
	gactive = false;
	TimerPool::instance().deactivateAll();
}

Timer::ts_t Timer::timestamp(void)
{
	struct timespec ts;
	ts_t t;

	clock_gettime(CLOCK_REALTIME, &ts);

	t = ts.tv_sec * (ts_t)1000000000L + ts.tv_nsec;

	return t;
}

// # ---------------------------------------------------------------------------

Timer::ts_t Timer::lapse(void)
{
	ts_t t = t_;
	t = (t_ = timestamp()) - t;
	return t;
}

Timer::ts_t Timer::get(void)
{
	return timestamp() - t_;
}

void Timer::thread(Time t, Ftor_ptr f)
{
	cond_.lock();

	struct timespec ts;
	ts_t o = 0;
	do
	{
		if(o != t_)
		{
			o = t_;
			ts_t d = get() / 1000;
			if(d > t.raw()) break;
			Time::us(t.raw() - d).toTimespec(&ts);
		}

		if(gactive && active() && cond_.wait(&ts))
		{
			(*f)();
			if(o == t_) break;
		}
	}
	while(gactive && active());

	cond_.unlock();
}

// # ===========================================================================

void TimerPoolImpl::deactivateAll(void)
{
	typedef std::vector<Timer *>::iterator iter_t;

	for(iter_t i1 = timers_.begin(), i2 = timers_.end() ; i1 != i2 ; ++i1)
	{
		(*i1)->deactivate();
	}
}

}

