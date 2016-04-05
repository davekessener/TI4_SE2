#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME RingBufferTest
#endif

#include <unistd.h>
#include <iostream>

#include <test/unit_test.hpp>

#include <RingBuffer.hpp>
#include <concurrent/Thread.h>
#include <concurrent/Mutex.h>

BEGIN

typedef lib::RingBuffer<int, 5, lib::RingBufferConcurrency::MultiThreaded> rb_t;

struct Flush { } flush;
struct Logger
{
	Logger( ) : locked_(false) { }

	template<typename T>
	Logger& operator<<(const T& t)
	{
		if(!locked_)
		{
			mtx_.lock();
			locked_ = true;
		}

		std::cout << t;

		return *this;
	}

	Logger& operator<<(const Flush&)
	{
		std::cout << std::endl;
		locked_ = false;
		mtx_.unlock();

		return *this;
	}

	lib::Mutex mtx_;
	bool locked_;
} log;

class Producer
{
	public:
		Producer(const std::string& n, rb_t *rb) : name_(n), rb_(rb) { }
		void operator()( )
		{
			for(int i = 0 ; i < 10 ; ++i)
			{
				rb_->enqueue(i);
				log << "Producer " << name_ << " added " << i << flush;
				sleep(2);
			}
		}
	private:
		std::string name_;
		rb_t *rb_;
};

class Consumer
{
	public:
		Consumer(rb_t *rb) : rb_(rb) { }
		void operator()( )
		{
			while(true)
			{
				int v = rb_->dequeue();

				log << "Consumer removed " << v << flush;

				sleep(1);

				if(v == 9) break;
			}
		}
	private:
		rb_t *rb_;
};

TEST(ringbuffer)
{
	rb_t rb;

	Producer p1("A", &rb);
	Producer p2("B", &rb);
	Consumer c(&rb);
	lib::Thread tp1(p1), tp2(p2), tc(c);

	tp1.join();
	tp2.join();
	tc.join();
}

END

