#ifndef DAV_TEST_UNITTEST_H
#define DAV_TEST_UNITTEST_H

#include <string>

#define MXT_COUNTARGS_(a, b, c, d, e, f, g, h, i, j, N, ...) N
#define MXT_COUNTARGS(...) MXT_COUNTARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define MXT_CONCAT_(a, b) a##b
#define MXT_CONCAT(a, b) MXT_CONCAT_(a, b)

#define MXT_OVERLOAD(m, ...) MXT_CONCAT(m, MXT_COUNTARGS(__VA_ARGS__))(__VA_ARGS__)

#define ASSERT_TRUE_2(f, msg) assert_true(f, __FILE__, __LINE__, msg)
#define ASSERT_TRUE_1(f) ASSERT_TRUE_2(f, NULL)
#define ASSERT_TRUE(...) MXT_OVERLOAD(ASSERT_TRUE_, __VA_ARGS__)

namespace lib
{
	namespace test
	{
		class UnitTest
		{
			public:
				virtual ~UnitTest( ) { }
				void assert_true(bool, const std::string&, int, const char * = NULL);
		};
	}
}

#endif

