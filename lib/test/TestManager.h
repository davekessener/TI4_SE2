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
					virtual std::string read( );
					virtual bool isEmpty( ) const;
				};

			public:
				static TestManager& Instance( );

				void setUnit(const std::string&);
				void addTest(const std::string&, testFn);
				int run( );
				Log& getLog( );
			private:
				TestManager( );
				~TestManager( );
			private:
				class TestManagerImpl;
				TestManagerImpl *impl_;
		};
	}
}

#endif

