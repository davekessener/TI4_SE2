#ifndef DAV_TEST_MAIN_H
#define DAV_TEST_MAIN_H

#include <iostream>
#include "TestManager.h"

int main(int argc, char *argv[])
{
	using lib::test::TestManager;

	TestManager::Log& log(TestManager::Instance().getLog());
	int r = TestManager::Instance().run();

	while(!log.isEmpty())
	{
		std::cout << log.read() << std::endl;
	}

	return r;
}

#endif

