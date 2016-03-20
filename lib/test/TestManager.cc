#include <map>
#include <deque>
#include <sstream>
#include "TestManager.h"

namespace lib { namespace test {

class Logger : public TestManager::Log
{
	typedef std::deque<std::string> queue_t;
	typedef queue_t::iterator iter_t;
	typedef queue_t::const_iterator citer_t;
	struct flush_t { };

	public:
		template<typename T>
			Logger& operator<<(const T&);
		Logger& operator<<(const flush_t&);
		Logger& operator<<(const Logger&);
		void write(const std::string&);
		virtual std::string read( );
		virtual bool isEmpty( ) const { return log_.empty(); }

		static flush_t flush;
	private:
		queue_t log_;
		std::stringstream buf_;
};

Logger::flush_t Logger::flush;

class TestManager::TestManagerImpl
{
	friend class TestManager;
	typedef std::map<std::string, testFn> tests_t;
	typedef std::map<std::string, tests_t> units_t;

	units_t units_;
	tests_t *tests_;
	std::string cur_unit_;
	Logger log_;

	tests_t *getUnit(const std::string&);
};

void TestManager::setUnit(const std::string& id)
{
	impl_->tests_ = impl_->getUnit(id);
}

void TestManager::addTest(const std::string& id, testFn test)
{
	if(!impl_->tests_)
		throw std::string("ERR: no unit selected!");

	if(impl_->tests_->find(id) != impl_->tests_->end())
		throw std::string("ERR: tried to register duplicate tests: " + id);

	(*impl_->tests_)[id] = test;
}

int TestManager::run(void)
{
	typedef TestManagerImpl::units_t::iterator uiter_t;
	typedef TestManagerImpl::tests_t::iterator titer_t;

	Logger err;
	Logger& log(impl_->log_);

	for(uiter_t i1 = impl_->units_.begin(), i2 = impl_->units_.end() ; i1 != i2 ; ++i1)
	{
		log << i1->first << ": ";

		for(titer_t j1 = i1->second.begin(), j2 = i1->second.end() ; j1 != j2 ; ++j1)
		{
			try
			{
				(*j1->second)();

				log << ".";
			}
			catch(const std::string& e)
			{
				err << i1->first << "#" << j1->first << ": " << e << Logger::flush;

				log << "F";
			}
		}

		log << Logger::flush;
	}

	log << err;

	return 0;
}

TestManager::Log& TestManager::getLog(void)
{
	return impl_->log_;
}

TestManager::TestManager(void) : impl_(new TestManagerImpl)
{
	impl_->tests_ = NULL;
}

TestManager::~TestManager(void)
{
	delete impl_;
}

TestManager& TestManager::Instance(void)
{
	static TestManager tm;

	return tm;
}

TestManager::TestManagerImpl::tests_t *TestManager::TestManagerImpl::getUnit(const std::string& id)
{
	if(id == cur_unit_ || units_.find(id) != units_.end())
		throw std::string("ERR: unit named '" + id + "' already exists!");

	cur_unit_ = id;

	return &units_[cur_unit_];
}

// # ---------------------------------------------------------------------------

std::string TestManager::Log::read(void)
{
	throw std::string("ERR: this is not supposed to be executed!");
}

bool TestManager::Log::isEmpty(void) const
{
	return true;
}

void Logger::write(const std::string& s)
{
	log_.push_back(s);
}

std::string Logger::read(void)
{
	std::string s(log_.front());
	log_.pop_front();
	return s;
}

template<typename T>
Logger& Logger::operator<<(const T& t)
{
	buf_ << t;
	return *this;
}

Logger& Logger::operator<<(const flush_t&)
{
	write(buf_.str());
	buf_.str("");
	return *this;
}

Logger& Logger::operator<<(const Logger& log)
{
	for(citer_t i1 = log.log_.begin(), i2 = log.log_.end() ; i1 != i2 ; ++i1)
	{
		log_.push_back(*i1);
	}

	return *this;
}

}}

