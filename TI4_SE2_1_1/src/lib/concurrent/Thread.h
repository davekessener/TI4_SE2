/***************************************************************************
*    Embedded Programming HAW Hamburg Department Informatik
*    HAW:: Thread.h
*    The HAW::Thread class implements the wrapper facade pattern by              
*    D. Schmidt POSA II. In this implementation it wraps the pthread_create call 
*    used by QNX to create threads.
*    COPYRIGHT (C) 2016 Stephan Pareigis
*    This software is licensed under MIT license.                                                                   
****************************************************************************/

#ifndef LIB_HAW_THREAD_H
#define LIB_HAW_THREAD_H

#include <stdexcept>
#include <memory>

#include <pthread.h>

#include <mpl/types.hpp>

/**
* Thread class.
* Encapsulates the most important features of a thread.
* This serves as a basis for further development.
* For example, priorities could be passed as constructor 
* argument.
*/
namespace lib
{
	class Thread
	{
		private:
			struct Functor
			{
				virtual void operator()( ) = 0;
			};
			
			template<typename F>
			class ActualFunctor : public Functor
			{
				public:
					ActualFunctor(const F& f) : f_(f) { }
					void operator()( ) { f_(); }
				private:
					F f_;
			};

		public:
			/**
			 * Default constructor. Initializes inert Thread
			 */
			Thread( );

			/**
			 * Constructor taking functor to execute in new thread.
			 * @warning throws std::runtime_error if thread cannot be started.
			 */
			template<typename F>
				Thread(F);

			/**
			 * Copy constructor;
			 * Moves content to this Thread object.
			 */
			Thread(Thread&);

			/**
			 * Destructor.
			 * @warning terminates if this Thread is still joinable
			 */
			~Thread( );

			/**
			 * Assignment operator.
			 * Moves content to this Thread object.
			 * @warning terminates if this Thread is already joinable
			 */
			Thread& operator=(Thread&);

			/**
			 * Calls join on the Thread.
			 * @warning must be called from the same context as ctor.
			 * @warning throws std::runtime_error if this Thread is not joinable
			 */
			void join( );

			/**
			 * Wether or not the Thread is joinable.
			 * @warning Thread cannot be destroyed while joinable
			 */
			bool joinable( ) const { return joinable_; }

		protected:
			/**
			 * This is called from the new Thread.
			 * It executes the user's functor.
			 * @warning terminates if functor throws an exception
			 */
			void run( );

			static void *entryPoint(void *);

		private:
			Thread(const Thread&); // Default copy constructor deleted
			Thread& operator=(const Thread&); // Default assignment operator deleted

		private:
			pthread_t tid_;/**< Thread id to be passed to pthread_create*/
			std::auto_ptr<Functor> f_;
			bool joinable_;
	};

	template<typename F>
	Thread::Thread(F f)
		: f_(new ActualFunctor<typename Decay<F>::Type>(f))
		, joinable_(false)
	{
		int err = pthread_create(&tid_, NULL, &Thread::entryPoint, this);

		if(err)
			throw std::runtime_error("Thread could not be started.");

		joinable_ = true;
	}
}

#endif
