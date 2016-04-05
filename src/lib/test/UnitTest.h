#ifndef DAV_TEST_UNITTEST_H
#define DAV_TEST_UNITTEST_H

#include <string>
#include <aux.h>

#define ASSERT_TRUE_2(f, msg) ::lib::test::UnitTest::assert_true(f, __FILE__, __LINE__, msg)
#define ASSERT_TRUE_1(f) ASSERT_TRUE_2(f, NULL)
#define ASSERT_TRUE(...) MXT_OVERLOAD(ASSERT_TRUE_, __VA_ARGS__)

namespace lib
{
	namespace test
	{
		class UnitTest
		{
			public:
				static void assert_true(bool, const std::string&, int, const char * = NULL);
				static void assert_true(bool f, const std::string& s, int l, const std::string& m)
					{ assert_true(f, s, l, m.c_str()); }
			private:
				UnitTest( );
				UnitTest(const UnitTest&);
				~UnitTest( );
				UnitTest& operator=(const UnitTest&);
		};
	}
}

#endif

