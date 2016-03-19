#include <iostream>
#include "fsm.hpp"

struct Store
{
	int i_;
};

typedef Store *pStore;

struct A
{
	static void enter(pStore s)
	{
		std::cout << "enter A: " << ++(s->i_) << std::endl;
	}
};

struct B
{
	static void enter(pStore s)
	{
		std::cout << "enter B: " << ++(s->i_) << std::endl;
	}

	static void leave(pStore s)
	{
		std::cout << "leave B: " << ++(s->i_) << std::endl;
	}
};

struct Ea { };
struct Eb { };

struct Rule_AaB
{
	template<typename E>
	static void apply(const E& e, pStore s)
	{
		std::cout << "A -a> B: " << ++(s->i_) << std::endl;
	}
};

//template<>
//struct Rule<A, Eb, A>
//{
//	static void apply(const Eb& e)
//	{
//		std::cout << "A -a> B" << std::endl;
//	}
//};

struct Rule_BaA
{
	static void apply(const Ea& e, pStore s)
	{
		std::cout << "B -a> A: " << ++(s->i_) << std::endl;
	}
};

struct Rule_BbA
{
	static void apply(const Eb& e, pStore s)
	{
		std::cout << "B -b> A: " << ++(s->i_) << std::endl;
	}
};

typedef lib::MakeFSM
<
	A,
	pStore,
	lib::MakeList
	<
		lib::Rule<A, Ea, B, Rule_AaB>,
		lib::Rule<A, Eb, A>,
		lib::Rule<B, Ea, A, Rule_BaA>,
		lib::Rule<B, Eb, A, Rule_BbA>
	>
>
MyFSM;

int main(int argc, char *argv[])
{
	Store s;
	MyFSM fsm(&s);

	s.i_ = 0;

	fsm.process(Ea());
	fsm.process(Eb());
	fsm.process(Eb());

	return 0;
}

