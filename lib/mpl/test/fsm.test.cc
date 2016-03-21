#ifndef TEST_CASE_NAME
#	define TEST_CASE_NAME MapTest
#endif

#include <test/unit_test.hpp>
#include <mpl/fsm.hpp>
#include <deque>
#include <sstream>
#include <string>

namespace
{
// # ---------------------------------------------------------------------------

	enum
	{
		enterA,
		enterB,
		leaveB,
		trans_AaB,
		trans_AbA,
		trans_BbA
	};

// # ---------------------------------------------------------------------------
	
	class Store
	{
		public:
			Store( );
			bool check(int);
			std::string msg( ) const;
			void setNextEvent(const std::string& e) { e_ = e; }
		private:
			std::deque<int> trace_;
			std::string e_;
			int c_;
	};

	Store::Store( )
	{
		trace_.push_back(enterA);

//		leaveA
		trace_.push_back(trans_AaB);
		trace_.push_back(enterB);
		
		trace_.push_back(leaveB);
		trace_.push_back(trans_BbA);
		trace_.push_back(enterA);
		
		trace_.push_back(trans_AbA);
		
//		leaveA
		trace_.push_back(trans_AaB);
		trace_.push_back(enterB);

		trace_.push_back(leaveB);
//		BaA
		trace_.push_back(enterA);

		c_ = trace_.size();
	}

	bool Store::check(int e)
	{
		bool r = trace_.front() == e;
		trace_.pop_front();
		return r;
	}

	std::string Store::msg(void) const
	{
		std::stringstream ss;

		ss << "upon processing event " << e_ << ", element #" << c_ - trace_.size() << " failed";

		return ss.str();
	}
	
	typedef Store *pStore;

// # ---------------------------------------------------------------------------

	struct A
	{
		static void enter(pStore s)
		{
			ASSERT_TRUE((s->check(enterA)), s->msg());
		}
	};
	
	struct B
	{
		static void enter(pStore s)
		{
			ASSERT_TRUE((s->check(enterB)), s->msg());
		}
	
		static void leave(pStore s)
		{
			ASSERT_TRUE((s->check(leaveB)), s->msg());
		}
	};

// # ---------------------------------------------------------------------------
	
	struct Ea { };
	struct Eb { };

// # ---------------------------------------------------------------------------
	
#define CREATE_RULE(trans) \
	struct Rule_##trans \
	{ \
		template<typename E> \
		static void apply(const E& e, pStore s) \
		{ \
			ASSERT_TRUE((s->check(trans_##trans)), s->msg()); \
		} \
	}

	CREATE_RULE(AaB);
	CREATE_RULE(AbA);
	CREATE_RULE(BbA);

// # ---------------------------------------------------------------------------
	
	typedef DO(lib::FSMMaker
	<
		A,
		pStore,
		DO(lib::MakeList_4
		<
			lib::Rule<A, Ea, B, Rule_AaB>,
			lib::Rule<A, Eb, A, Rule_AbA>,
			lib::Rule<B, Ea, A>,
			lib::Rule<B, Eb, A, Rule_BbA>
		>)
	>)
	MyFSM;
}

// # ===========================================================================

BEGIN

TEST(fsm_trace)
{
	Store s;
	MyFSM fsm(&s);

#define PR(e) s.setNextEvent(#e); fsm.process(e())
	PR(Ea);
	PR(Eb);
	PR(Eb);
	PR(Ea);
	PR(Ea);
#undef PR
}

END

