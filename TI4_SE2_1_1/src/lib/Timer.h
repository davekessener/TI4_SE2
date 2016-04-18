#ifndef HAW_LIB_TIMER_H
#define HAW_LIB_TIMER_H

#include <SmartPtr.hpp>
#include <mpl/FtorWrapper.hpp>
#include <concurrent/Condition.h>

namespace lib
{
	class Timer
	{
		typedef SmartPtr<BasicFunctor> Ftor_ptr;

		template<typename F>
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
			void removeTimer(Timer *t) { timers_.remove(timers_.find(t)); }
			void deactivateAll( );

		private:
			std::vector<Timer *> timers_;

		private:
			TimerPoolImpl( ) { }
			TimerPoolImpl(const TimerPoolImpl&);
			~TimerPoolImpl( ) { }
			TimerPoolImpl& operator=(const TimerPoolImpl&);

			friend class SingletonInst;
			friend class Timer;
	};

	typedef TimerPoolImpl::SingletonInst TimerPool;

	template<typename F>
	void Timer::ExecuteWhen(Time t, F f)
	{
		if(active())
			deactivate();

		if(gactive)
			thread_.set(
				new Thread(
					Ftor(this, 
						 &Timer::thread, 
						 t, 
						 Ftor_ptr(new BasicFunctorImpl<typename Decay<F>::Type>(f)))));
	}
}

#endif

