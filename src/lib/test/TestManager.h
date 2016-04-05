#ifndef DAV_TEST_MANAGER_H
#define DAV_TEST_MANAGER_H


#include <string>

namespace lib
{
	namespace test
	{
		class TestManager
		{
			public:
				typedef void (*testFn)(void);

				struct Registrar
				{
					Registrar(const std::string& test_id, testFn test)
					{
						Instance().addTest(test_id, test);
					}
				};

				struct Selector
				{
					Selector(const std::string& unit_id)
					{
						Instance().setUnit(unit_id);
					}
				};

				struct Log
				{
					virtual std::string read( ) = 0;
					virtual bool isEmpty( ) const = 0;
				};

			public:
				static TestManager& Instance( );

				void setUnit(const std::string&);
				void addTest(const std::string&, testFn);
				int run( );
				Log& getLog( );
			private:
				TestManager( );
				TestManager(const TestManager&);
				~TestManager( );
				TestManager& operator=(const TestManager&);
			private:
				class TestManagerImpl;
				TestManagerImpl *impl_;
		};
	}
}

#endif

