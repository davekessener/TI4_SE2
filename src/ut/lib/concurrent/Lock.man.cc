#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME LockTest
#endif

#include <iostream>
#include <unistd.h>

#include <test/unit_test.hpp>

#include <concurrent/Thread.h>
#include <concurrent/Mutex.h>
#include <concurrent/Lock.hpp>

#define MXT_COUNT 3

BEGIN

class Holder : public lib::LockableObject<Holder>
{
	public:
		static Holder& instance() { static Holder inst; return inst; }

		void wait(const std::string& s) { Lock lock(this); std::cout << "Called by " << s << std::endl; sleep(1); }
	private:
		Holder( ) { }
		~Holder( ) { }
		Holder(const Holder&);
		Holder& operator=(const Holder&);
};

class Printer
{
	public:
		Printer(const std::string& s) : s_(s) { }
		void operator()( )
		{
			int i = MXT_COUNT;

			while(i > 0)
			{
				Holder::instance().wait(s_); --i;
			}
		}
	private:
		std::string s_;
};

TEST(locking)
{
	Holder::instance().wait("main");

	lib::Thread a(Printer("A")), b(Printer("B"));

	a.join();
	b.join();
}

END

