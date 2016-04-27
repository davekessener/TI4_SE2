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
/**
 * Timer that allows scheduling of functors.
 * The functors will be executed after a specific amount of time in their own thread.
 * By resetting the Timer from within the supplied functor a periodic execution
 * can be achieved.
 * 
 * Also allows for synchronisation to a specific time frame.
 */
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
			/** Synchronizes execution.
			 * By suspending the current thread until <tt>t</tt> amount of
			 * time has part since the Timer has been started/reset this
			 * function (if called within a loop) synchronizes the active
			 * threads execution to a specific frequency (\$f\frac{1}{t}\$f)
			 */
			void sync(Time t);
			void reset( );
			/** Amount of time elapsed since last reset. Resets timer. */
			Time delta( );
			/** Amount of time elapsed since last reset. Doesn't reset timer. */
			Time elapsed( );
			template<typename F>
			void executeWhen(Time, F);
			/** Is timer currently waiting for ftor execution. */
			bool active( ) const;
			void deactivate( );

			/** Deactivate all timers. Prevents timing issues during the applications
			 * termination (i.e. waiting during <tt>join</tt> for a timer).
			 */
			static void deactivateAll( );
			/** Returns current system time in nanoseconds since Jan. 1st 1970. */
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

