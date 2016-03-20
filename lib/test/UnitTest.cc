#include <sstream>
#include "UnitTest.h"

namespace lib { namespace test {

void UnitTest::assert_true(bool f, const std::string& file, int line, const char *msg)
{
	if(!f)
	{
		std::stringstream ss;

		ss << "assert_true failed in '" << file << ":" << line << "'";

		if(msg)
			ss << " ('" << msg << "')";

		ss << "!";

		throw std::string(ss.str());
	}
}

}}

