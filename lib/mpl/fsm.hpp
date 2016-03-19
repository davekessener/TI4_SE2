#ifndef DAV_MPL_FSM_H
#define DAV_MPL_FSM_H

#include "map.hpp"

namespace lib
{
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
	
	template<typename D>
	struct FSMBase
	{
		virtual ~FSMBase() { }
		int get_state() { return state_; }
		D get_data() { return data_; }
		void set_state(int state) { state_ = state; }
		void set_data(D d) { data_ = d; }
		
		int state_;
		D data_;
	};

	template<>
	struct FSMBase<void>
	{
		virtual ~FSMBase() { }
		int get_state() { return state_; }
		void set_state(int state) { state_ = state; }
		
		int state_;
	};
	
	template<typename O, typename E, typename D>
	struct Transition
	{
		typedef O Origin;
		typedef E Event;
		typedef D Destination;
	};

	template<typename O, typename E, typename D, typename A = Nil>
	struct Rule : public Transition<O, E, D>, public A
	{
	};

// ---------------------------------------------------------------------------

#define MXT_HASFN(f) \
template<typename T, typename D> \
struct TryCall_##f \
{ \
	template<typename, void (*)(D)> struct Check { }; \
	template<typename TT> static void test(D d, Check<TT, &TT::f>*) { TT::f(d); } \
	template<typename> static void test(D d, ...) { } \
	static void call(D d) { test<T>(d, nullptr); } \
}

	MXT_HASFN(enter);
	MXT_HASFN(leave);

#undef MXT_HASFN

	template<typename T, typename E, typename D>
	struct TryCall_apply
	{
		template<typename, void (*)(const E&, D)> struct Check { };
		template<typename TT> static void test(const E& e, D d, Check<TT, &TT::apply>*) { TT::apply(e, d); }
		template<typename> static void test(const E& e, D d, ...) { }
		static void call(const E& e, D d) { test<T>(e, d, nullptr); }
	};
	
	template<typename E, typename D, typename L, typename S, typename T>
	struct TransImpl : public TransImpl<E, D, Cdr<L>, S, T>
	{
		typedef TransImpl<E, D, Cdr<L>, S, T> Super;
		typedef E Event;
		typedef D Data;
		typedef S StateList;
		typedef T TransitionList;
		typedef Caar<L> Origin;
		typedef Cdar<L> Destination;
		typedef TryCall_leave<Origin, Data> LeaveFunction;
		typedef TryCall_enter<Destination, Data> EnterFunction;
		typedef TryCall_apply<DoGetValue<TransitionList, Transition<Origin, Event, Destination>>, Event, Data> TransitionFunction;
		static const int OriginID = DoGetValue<StateList, Origin>::value;
		static const int DestinationID = DoGetValue<StateList, Destination>::value;
		static const bool IsActualTransition = !IsSame<Origin, Destination>::value;
	
		using FSMBase<D>::get_state;
		using FSMBase<D>::get_data;
		using FSMBase<D>::set_state;
	
		virtual void process(const Event& e)
		{
			if(get_state() == OriginID)
			{
				Data d = get_data();

				if(IsActualTransition)
					LeaveFunction::call(d);
	
				TransitionFunction::call(e, d);
	
				if(IsActualTransition)
					EnterFunction::call(d);
	
				set_state(DestinationID);
			}
			else
			{
				Super::process(e);
			}
		}
	};
	
	template<typename E, typename D, typename S, typename T>
	struct TransImpl<E, D, Nil, S, T> : public virtual FSMBase<D>
	{
		virtual void process(const E& e)
		{
		}
	};
	
// ---------------------------------------------------------------------------
	
#define MXT_HASFN(f) \
template<typename T> \
struct TryCall_##f <T, void> \
{ \
	template<typename, void (*)(void)> struct Check { }; \
	template<typename TT> static void test(Check<TT, &TT::f>*) { TT::f(); } \
	template<typename> static void test(...) { } \
	static void call(void) { test<T>(nullptr); } \
}

	MXT_HASFN(enter);
	MXT_HASFN(leave);

#undef MXT_HASFN

	template<typename T, typename E>
	struct TryCall_apply<T, E, void>
	{
		template<typename, void (*)(const E&)> struct Check { };
		template<typename TT> static void test(const E& e, Check<TT, &TT::apply>*) { TT::apply(e); }
		template<typename> static void test(const E& e, ...) { }
		static void call(const E& e) { test<T>(e, nullptr); }
	};
	
	template<typename E, typename L, typename S, typename T>
	struct TransImpl<E, void, L, S, T> : public TransImpl<E, void, Cdr<L>, S, T>
	{
		typedef TransImpl<E, void, Cdr<L>, S, T> Super;
		typedef E Event;
		typedef S StateList;
		typedef T TransitionList;
		typedef Caar<L> Origin;
		typedef Cdar<L> Destination;
		typedef TryCall_leave<Origin, void> LeaveFunction;
		typedef TryCall_enter<Destination, void> EnterFunction;
		typedef TryCall_apply<DoGetValue<TransitionList, Transition<Origin, Event, Destination>>, Event, void> TransitionFunction;
		static const int OriginID = DoGetValue<StateList, Origin>::value;
		static const int DestinationID = DoGetValue<StateList, Destination>::value;
		static const bool IsActualTransition = !IsSame<Origin, Destination>::value;
	
		using FSMBase<void>::get_state;
		using FSMBase<void>::set_state;
	
		virtual void process(const Event& e)
		{
			if(get_state() == OriginID)
			{
				if(IsActualTransition)
					LeaveFunction::call();
	
				TransitionFunction::call(e);
	
				if(IsActualTransition)
					EnterFunction::call();
	
				set_state(DestinationID);
			}
			else
			{
				Super::process(e);
			}
		}
	};
	
	template<typename E, typename S, typename T>
	struct TransImpl<E, void, Nil, S, T> : public virtual FSMBase<void>
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

	template<typename List>
	struct CreateTransitionMap
	{
		template<typename T>
		struct Transform
		{
			typedef Cons
			<
				Transition
				<
					typename T::Origin,
					typename T::Event,
					typename T::Destination
				>,
				T
			>
			Type;
		};

		typedef DoApply<Transform, List> Type;
	};

	template<typename List>
	using DoCreateTransitionMap = typename CreateTransitionMap<List>::Type;
	
// ---------------------------------------------------------------------------
	
	template<int ID, typename D, typename Lineage>
	struct FSM : public Lineage
	{
		using Lineage::process;
	
		FSM(D d)
		{
			FSMBase<D>::set_state(ID);
			FSMBase<D>::set_data(d);
		}
	};
	
	template<int ID, typename Lineage>
	struct FSM<ID, void, Lineage> : public Lineage
	{
		using Lineage::process;
	
		FSM( ) { FSMBase<void>::set_state(ID); }
	};
	
	template<typename I, typename D, typename T>
	struct FSMMaker
	{
		typedef I InitialState;
		typedef D Data;
		typedef DoCreateStateList<T> StateList;
		typedef DoCreateTransitionDependencyList<T> Transitions;
		typedef DoCreateTransitionMap<T> TransitionMap;
		static const int InitialID = DoGetValue<StateList, InitialState>::value;
	
		template<typename TT>
		struct CreateTransitionTree
		{
			typedef TransImpl<Car<TT>, Data, Cadr<TT>, StateList, TransitionMap> Type;
		};
	
		typedef ConstructFSMLineage<DoApply<CreateTransitionTree, Transitions>> Lineage;
	
		typedef FSM<InitialID, Data, Lineage> Type;
	};
	
	template<typename I, typename D, typename T>
	using MakeFSM = typename FSMMaker<I, D, T>::Type;
}

#endif

