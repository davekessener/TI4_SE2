#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME ThreadTest
#endif

#include <iostream>

#include <test/unit_test.hpp>

#include <concurrent/Thread.h>

#define MXT_COUNT 5

BEGIN

class Keeper
{
	public:
		Keeper(int v, volatile int *i) : v_(v), i_(i) { }
		void operator() ( )
		{
			int i = MXT_COUNT;

			while(i > 0)
			{
				if(*i_ != v_)
				{
					std::cout << "Not " << v_ << "!" << std::endl;
					*i_ = v_;
					--i;
				}
			}
		}
	private:
		int v_;
		volatile int *i_;
};

TEST(threads)
{
	int *i = new int;

	*i = 5;

	Keeper five(5, i), seven(7, i);

	lib::Thread t5(five), t7(seven);

	t5.join();
	t7.join();
}

END

