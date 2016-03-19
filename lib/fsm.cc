#include <iostream>

struct Nil
{
	typedef Nil Type;
};

template<typename H, typename T>
struct Cons
{
	typedef H Head;
	typedef T Tail;
};

template<typename ... T>
struct ConsMaker;

template<typename T, typename ... TT>
struct ConsMaker<T, TT...>
{
	typedef Cons<T, typename ConsMaker<TT...>::Type> Type;
};

template<>
struct ConsMaker<>
{
	typedef Nil Type;
};

template<typename T1, typename T2>
struct ConsFn
{
	typedef Cons<T1, T2> Type;
};

template<typename ... T>
using MakeList = typename ConsMaker<T...>::Type;

template<typename T>
using Car = typename T::Head;

template<typename T>
struct CarFn
{
	typedef Car<T> Type;
};

template<typename T>
using Cdr = typename T::Tail;

template<typename T>
struct CdrFn
{
	typedef Cdr<T> Type;
};

template<typename T>
using Caar = Car<Car<T>>;

template<typename T>
struct CaarFn
{
	typedef Caar<T> Type;
};

template<typename T>
using Cadr = Car<Cdr<T>>;

template<typename T>
struct CadrFn
{
	typedef Cadr<T> Type;
};

template<typename T>
using Cdar = Cdr<Car<T>>;

template<typename T>
struct CdarFn
{
	typedef Cdar<T> Type;
};

template<typename T>
using Cddr = Cdr<Cdr<T>>;

template<typename T>
struct CddrFn
{
	typedef Cddr<T> Type;
};

template<int IDX, typename List>
struct GetElem
{
	typedef typename GetElem<IDX - 1, Cdr<List>>::Type Type;
};

template<typename List>
struct GetElem<0, List>
{
	typedef Car<List> Type;
};

template<int IDX, typename List>
using DoGetElem = typename GetElem<IDX, List>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct InheritLineage : public Car<T>, public InheritLineage<Cdr<T>>
{
};

template<>
struct InheritLineage<Nil>
{
};

template<typename ... T>
using DoInheritLineage = InheritLineage<MakeList<T...>>;

// ---------------------------------------------------------------------------

template<template <typename> class F, typename T>
struct Apply
{
	typedef Cons<typename F<Car<T>>::Type, typename Apply<F, Cdr<T>>::Type> Type;
};

template<template <typename> class F>
struct Apply<F, Nil>
{
	typedef Nil Type;
};

template<template <typename> class F, typename T>
using DoApply = typename Apply<F, T>::Type;

// ---------------------------------------------------------------------------

template<typename T1, typename T2>
struct IsSame
{
	static const bool value = false;
};

template<typename T>
struct IsSame<T, T>
{
	static const bool value = true;
};

// ---------------------------------------------------------------------------

template<typename T, T I>
struct Value
{
	static const T value = I;
};

template<int I>
struct Int : public Value<int, I>
{
};

template<bool V>
struct Bool : public Value<bool, V>
{
};

typedef Bool<true> True;
typedef Bool<false> False;

template<typename T>
struct Identity
{
	typedef T Type;
};

// ---------------------------------------------------------------------------

template<bool Cond, typename T1, typename T2>
struct If;

template<typename T1, typename T2>
struct If<true, T1, T2>
{
	typedef typename T1::Type Type;
};

template<typename T1, typename T2>
struct If<false, T1, T2>
{
	typedef typename T2::Type Type;
};

template<typename C, typename T1, typename T2>
using DoIf = typename If<C::value, T1, T2>::Type;

// ---------------------------------------------------------------------------

template<typename Map, typename Key>
struct GetValue
{
	typedef DoIf
	<
		IsSame<Caar<Map>, Key>,
		Identity<Cdar<Map>>,
		GetValue<Cdr<Map>, Key>
	> Type;
};

template<typename Map, typename Key>
using DoGetValue = typename GetValue<Map, Key>::Type;

// ---------------------------------------------------------------------------

template<int IDX, typename List>
struct ListToMapImpl
{
	typedef Cons
	<
		Cons<Int<IDX>, Car<List>>,
		typename ListToMapImpl<IDX + 1, Cdr<List>>::Type
	> Type;
};

template<int IDX>
struct ListToMapImpl<IDX, Nil>
{
	typedef Nil Type;
};

template<typename List>
struct ListToMap
{
	typedef typename ListToMapImpl<0, List>::Type Type;
};

template<typename List>
using DoListToMap = typename ListToMap<List>::Type;

// ---------------------------------------------------------------------------

template<typename Done, typename ToDo>
struct ReverseImpl
{
	typedef typename ReverseImpl
	<
		Cons<Car<ToDo>, Done>,
		Cdr<ToDo>
	>::Type
	Type;
};

template<typename Done>
struct ReverseImpl<Done, Nil>
{
	typedef Done Type;
};

template<typename List>
struct Reverse
{
	typedef typename ReverseImpl<Nil, List>::Type Type;
};

template<typename List>
using DoReverse = typename Reverse<List>::Type;

// ---------------------------------------------------------------------------

template<typename Cell>
struct ReverseCons
{
	typedef Cons<Cdr<Cell>, Car<Cell>> Type;
};

template<typename Cell>
using DoReverseCons = typename ReverseCons<Cell>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct IsList
{
	static const bool value = false;
};

template<typename T1, typename T2>
struct IsList<Cons<T1, T2>>
{
	static const bool value = true;
};

// ---------------------------------------------------------------------------

template<typename List, typename Appendage>
struct Append
{
	typedef Cons<Car<List>, typename Append<Cdr<List>, Appendage>::Type> Type;
};

template<typename Appendage>
struct Append<Nil, Appendage>
{
	typedef Appendage Type;
};

template<typename List, typename Appendage>
using DoAppend = typename Append<List, Appendage>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct Flatten
{
	typedef T Type;
};

template<typename H, typename T>
struct Flatten<Cons<H, T>>
{
	typedef typename Flatten<T>::Type Rest;

	typedef DoIf
	<
		IsList<H>,
		Append<typename Flatten<H>::Type, Rest>,
		Identity<Cons<H, Rest>>
	>
	Type;
};

template<typename List>
using DoFlatten = typename Flatten<List>::Type;

// ---------------------------------------------------------------------------

template<template <typename, typename> class F, typename T, typename List>
struct Merge
{
	typedef typename Merge<F, typename F<T, Car<List>>::Type, Cdr<List>>::Type Type;
};

template<template <typename, typename> class F, typename T>
struct Merge<F, T, Nil>
{
	typedef T Type;
};

template<template <typename, typename> class F, typename T, typename List>
using DoMerge = typename Merge<F, T, List>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct Not
{
	static const bool value = !T::value;
};

template<typename T1, typename T2>
struct And
{
	static const bool value = T1::value && T2::value;
};

template<typename List>
struct ListAnd
{
	template<typename T1, typename T2>
	struct AndFn
	{
		typedef And<T1, T2> Type;
	};

	typedef typename Merge<AndFn, True, List>::Type Type;
	static const bool value = Type::value;
};

template<typename T1, typename T2>
struct Or
{
	static const bool value = T1::value || T2::value;
};

template<typename List>
struct ListOr
{
	template<typename T1, typename T2>
	struct OrFn
	{
		typedef Or<T1, T2> Type;
	};

	typedef typename Merge<OrFn, False, List>::Type Type;
	static const bool value = Type::value;
};

// ---------------------------------------------------------------------------

template<typename List, typename T>
struct Contains
{
	static const bool value = IsSame<Car<List>, T>::value || Contains<Cdr<List>, T>::value;
};

template<typename T>
struct Contains<Nil, T>
{
	static const bool value = false;
};

// ---------------------------------------------------------------------------

template<typename Done, typename ToDo>
struct SetifyImpl
{
	typedef typename SetifyImpl
	<
		DoIf
		<
			Contains<Done, Car<ToDo>>,
			Identity<Done>,
			Identity<Cons<Car<ToDo>, Done>>
		>,
		Cdr<ToDo>
	>::Type
	Type;
};

template<typename Done>
struct SetifyImpl<Done, Nil>
{
	typedef DoReverse<Done> Type;
};

template<typename List>
struct Setify
{
	typedef typename SetifyImpl<Nil, List>::Type Type;
};

template<typename List>
using DoSetify = typename Setify<List>::Type;

// ---------------------------------------------------------------------------

template<template <typename> class F, typename List>
struct Filter
{
	typedef typename Filter<F, Cdr<List>>::Type Rest;

	typedef DoIf
	<
		F<Car<List>>,
		Identity<Cons<Car<List>, Rest>>,
		Identity<Rest>
	>
	Type;
};

template<template <typename> class F>
struct Filter<F, Nil>
{
	typedef Nil Type;
};

template<template <typename> class F, typename List>
using DoFilter = typename Filter<F, List>::Type;

// ---------------------------------------------------------------------------

template<typename List>
struct CreateStateList
{
	template<typename T>
	struct DoGetStateFromTransition
	{
		typedef MakeList<typename T::Origin, typename T::Destination> Type;
	};

	typedef DoFlatten<DoApply<DoGetStateFromTransition, List>> StateList;
	typedef DoListToMap<DoSetify<StateList>> StateMap;
	typedef DoApply<ReverseCons, StateMap> Type;
};

template<typename List>
using DoCreateStateList = typename CreateStateList<List>::Type;

// ---------------------------------------------------------------------------

template<typename List>
struct CreateTransitionDependencyList
{
	template<typename T>
	struct DoGetDependency
	{
		typedef MakeList<typename T::Event, Cons<typename T::Origin, typename T::Destination>> Type;
	};

	typedef DoApply<DoGetDependency, List> RawDependencies;

	template<typename E>
	struct CollectDependencies
	{
		template<typename T>
		using IsCorrectEvent = IsSame<Car<T>, E>;

		typedef DoApply<CadrFn, DoFilter<IsCorrectEvent, RawDependencies>> Dependencies;
		typedef MakeList<E, Dependencies> Type;
	};

	typedef DoSetify<DoApply<CarFn, RawDependencies>> EventList;
	typedef DoApply<CollectDependencies, EventList> Type;
};

template<typename List>
using DoCreateTransitionDependencyList = typename CreateTransitionDependencyList<List>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct State
{
	typedef T Data;

	static void enter(Data) { std::cout << "enter base" << std::endl; }
	static void leave(Data) { std::cout << "leave base" << std::endl; }
};

template<>
struct State<void>
{
	typedef void Data;

	static void enter(void) { std::cout << "enter base" << std::endl; }
	static void leave(void) { std::cout << "leave base" << std::endl; }
};

template<class Event, typename Data>
struct RuleImpl
{
	static void apply(const Event&, Data) { std::cout << "empty transition" << std::endl; }
};

template<class Event>
struct RuleImpl<Event, void>
{
	static void apply(const Event&) { std::cout << "empty transition" << std::endl; }
};

template<class Origin, class Event, class Destination>
struct Rule : public RuleImpl<Event, typename Origin::Data>
{
	static_assert(IsSame<typename Origin::Data, typename Destination::Data>::value, "States need to have identical data-types!");
};

template<class O, class E, class D>
struct Transition
{
	typedef O Origin;
	typedef E Event;
	typedef D Destination;
};

struct FSMBase
{
	virtual ~FSMBase() { }
	int get_state() { return state_; }
	void set_state(int state) { state_ = state; }
	int state_;
};

// ---------------------------------------------------------------------------

template<typename E, typename D, typename L, typename S>
struct TransImpl : public TransImpl<E, D, Cdr<L>, S>
{
	typedef TransImpl<E, D, Cdr<L>, S> Super;
	typedef E Event;
	typedef D Data;
	typedef S StateList;
	typedef Car<L> Transition;
	typedef Car<Transition> Origin;
	typedef Cdr<Transition> Destination;
	typedef Rule<Origin, Event, Destination> TransitionFunction;
	static const int OriginID = DoGetValue<StateList, Origin>::value;
	static const int DestinationID = DoGetValue<StateList, Destination>::value;
	static const bool IsActualTransition = !IsSame<Origin, Destination>::value;

	using FSMBase::get_state;
	using FSMBase::set_state;

	virtual void process(const Event& e, Data d)
	{
		if(get_state() == OriginID)
		{
			if(IsActualTransition)
				Origin::leave(d);

			TransitionFunction::apply(e, d);

			if(IsActualTransition)
				Destination::enter(d);

			set_state(DestinationID);
		}
		else
		{
			Super::process(e, d);
		}
	}
};

template<typename E, typename D, typename S>
struct TransImpl<E, D, Nil, S> : public virtual FSMBase
{
	virtual void process(const E& e, D d)
	{
		throw new std::string("ERR: transition does not exist!");
	}
};

template<typename E, typename L, typename S>
struct TransImpl<E, void, L, S> : public TransImpl<E, void, Cdr<L>, S>
{
	typedef TransImpl<E, void, Cdr<L>, S> Super;
	typedef E Event;
	typedef S StateList;
	typedef Car<L> Transition;
	typedef Car<Transition> Origin;
	typedef Cdr<Transition> Destination;
	typedef Rule<Origin, Event, Destination> TransitionFunction;
	static const int OriginID = DoGetValue<StateList, Origin>::value;
	static const int DestinationID = DoGetValue<StateList, Destination>::value;
	static const bool IsActualTransition = !IsSame<Origin, Destination>::value;

	using FSMBase::get_state;
	using FSMBase::set_state;

	virtual void process(const Event& e)
	{
		if(get_state() == OriginID)
		{
			if(IsActualTransition)
				Origin::leave();

			TransitionFunction::apply(e);

			if(IsActualTransition)
				Destination::enter();

			set_state(DestinationID);
		}
		else
		{
			Super::process(e);
		}
	}
};

template<typename E, typename S>
struct TransImpl<E, void, Nil, S> : public virtual FSMBase
{
	virtual void process(const E& e)
	{
		throw new std::string("ERR: transition does not exist!");
	}
};

// ---------------------------------------------------------------------------

template<typename T>
struct ConstructFSMLineage : public Car<T>, public ConstructFSMLineage<Cdr<T>>
{
	using Car<T>::process;
	using ConstructFSMLineage<Cdr<T>>::process;
};

template<typename T>
struct ConstructFSMLineage<Cons<T, Nil>> : public T
{
	using T::process;
};

template<int ID, typename Lineage>
struct FSM : public Lineage
{
	using Lineage::process;

	FSM() { FSMBase::set_state(ID); }
};

template<typename I, typename T>
struct FSMMaker
{
	typedef I InitialState;
	typedef typename I::Data Data;
	typedef DoCreateStateList<T> StateList;
	typedef DoCreateTransitionDependencyList<T> Transitions;
	static const int InitialID = DoGetValue<StateList, InitialState>::value;

	template<typename TT>
	struct CreateTransitionTree
	{
		typedef TransImpl<Car<TT>, Data, Cadr<TT>, StateList> Type;
	};

	typedef ConstructFSMLineage<DoApply<CreateTransitionTree, Transitions>> Lineage;

	typedef FSM<InitialID, Lineage> Type;
};

template<typename I, typename T>
using MakeFSM = typename FSMMaker<I, T>::Type;

// ---------------------------------------------------------------------------

struct A : public State<void>
{
	static void enter(void)
	{
		std::cout << "enter A" << std::endl;
	}
};

struct B : public State<void>
{
	static void enter(void)
	{
		std::cout << "enter B" << std::endl;
	}

	static void leave(void)
	{
		std::cout << "leave B" << std::endl;
	}
};

struct Ea { };
struct Eb { };

template<>
struct Rule<A, Ea, B>
{
	static void apply(const Ea& e)
	{
		std::cout << "A -a> B" << std::endl;
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

template<>
struct Rule<B, Ea, A>
{
	static void apply(const Ea& e)
	{
		std::cout << "B -a> A" << std::endl;
	}
};

template<>
struct Rule<B, Eb, A>
{
	static void apply(const Eb& e)
	{
		std::cout << "B -b> A" << std::endl;
	}
};

typedef MakeFSM
<
	A,
	MakeList
	<
		Transition<A, Ea, B>,
		Transition<A, Eb, A>,
		Transition<B, Ea, A>,
		Transition<B, Eb, A>
	>
>
MyFSM;

int main(int argc, char *argv[])
{
	MyFSM fsm;

	fsm.process(Ea());
	fsm.process(Eb());
	fsm.process(Eb());

	return 0;
}

