#ifndef HAW_LIB_TIMER_H
#define HAW_LIB_TIMER_H

#include <memory>
#include <vector>
#include <algorithm>

#include <lib/SmartPtr.hpp>
#include <lib/mpl/FtorWrapper.hpp>
#include <lib/concurrent/Condition.h>
#include <lib/concurrent/Thread.h>
#include <lib/Singleton.hpp>

namespace lib
{
	class Timer
	{
		typedef SmartPtr<BasicFunctor> Ftor_ptr;

		struct Ftor
		{
			typedef void (Timer::*mf_fn)(Time, Ftor_ptr);
			Ftor(Timer *e, mf_fn m, Time t, Ftor_ptr f)
				: e_(e), m_(m), t_(t), f_(f) { }
			void operator()( ) { (e_->*m_)(t_, f_); }
			Timer *e_;
			mf_fn m_;
			Time t_;
			Ftor_ptr f_;
		};

		public:
			typedef uint64_t ts_t;

		public:
			Timer( );
			~Timer( );
			void sync(Time);
			void reset( );
			Time delta( );
			Time elapsed( );
			template<typename F>
			void executeWhen(Time, F);
			bool active( ) const;
			void deactivate( );

			static void deactivateAll( );
			static ts_t timestamp( );

		private:
			ts_t lapse( );
			ts_t get( );
			void thread(Time, Ftor_ptr);

		private:
			volatile ts_t t_;
			std::auto_ptr<Thread> thread_;
			Condition cond_;

			static bool gactive;
	};

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

	template<typename F>
	void Timer::executeWhen(Time t, F f)
	{
		if(active())
			deactivate();

		cond_.lock();

		if(gactive)
			thread_.reset(
				new Thread(
					Ftor(this, 
						 &Timer::thread, 
						 t, 
						 Ftor_ptr(new BasicFunctorImpl<typename Decay<F>::Type>(f)))));

		cond_.unlock();
	}
}

#endif

