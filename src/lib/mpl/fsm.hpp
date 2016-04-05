#ifndef DAV_MPL_FSM_H
#define DAV_MPL_FSM_H

#include <mpl/map.hpp>

namespace lib
{
	template<typename List>
	struct CreateStateList
	{
		template<typename T>
		struct GetStateFromTransition
		{
			typedef MAKELIST(typename T::Origin, typename T::Destination) Type;
		};
	
		typedef DO(Flatten<DO(Apply<GetStateFromTransition, List>)>) StateList;
		typedef DO(ListToMap<DO(Setify<StateList>)>) StateMap;
		typedef DO(Apply<ReverseCons, StateMap>) Type;
	};
	
// ---------------------------------------------------------------------------
	
	template<typename List>
	struct CreateTransitionDependencyList
	{
		template<typename T>
		struct GetDependency
		{
			typedef Cons<typename T::Origin, typename T::Destination> Tmp;
			typedef MAKELIST(typename T::Event, Tmp) Type;
		};
	
		typedef DO(Apply<GetDependency, List>) RawDependencies;
	
		template<typename E>
		struct CollectDependencies
		{
			template<typename T> struct IsCorrectEvent : public IsSame<DO(Car<T>), E> { };
	
			typedef DO(Apply<Cadr, DO(Filter<IsCorrectEvent, RawDependencies>)>) Dependencies;
			typedef MAKELIST(E, Dependencies) Type;
		};
	
		typedef DO(Setify<DO(Apply<Car, RawDependencies>)>) EventList;
		typedef DO(Apply<CollectDependencies, EventList>) Type;
	};
	
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
	static void call(D d) { test<T>(d, NULL); } \
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
		static void call(const E& e, D d) { test<T>(e, d, NULL); }
	};
	
	template<typename E, typename D, typename L, typename S, typename T>
	struct TransImpl : public TransImpl<E, D, DO(Cdr<L>), S, T>
	{
		typedef TransImpl<E, D, DO(Cdr<L>), S, T> Super;
		typedef E Event;
		typedef D Data;
		typedef S StateList;
		typedef T TransitionList;
		typedef DO(Caar<L>) Origin;
		typedef DO(Cdar<L>) Destination;
		typedef TryCall_leave<Origin, Data> LeaveFunction;
		typedef TryCall_enter<Destination, Data> EnterFunction;
		typedef TryCall_apply<DO(GetValue<TransitionList, Transition<Origin, Event, Destination> >), Event, Data> TransitionFunction;
		static const int OriginID = ValueIdentity<DO(GetValue<StateList, Origin>)>::value;
		static const int DestinationID = ValueIdentity<DO(GetValue<StateList, Destination>)>::value;
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
	static void call(void) { test<T>(NULL); } \
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
		static void call(const E& e) { test<T>(e, NULL); }
	};
	
	template<typename E, typename L, typename S, typename T>
	struct TransImpl<E, void, L, S, T> : public TransImpl<E, void, DO(Cdr<L>), S, T>
	{
		typedef TransImpl<E, void, DO(Cdr<L>), S, T> Super;
		typedef E Event;
		typedef S StateList;
		typedef T TransitionList;
		typedef DO(Caar<L>) Origin;
		typedef DO(Cdar<L>) Destination;
		typedef TryCall_leave<Origin, void> LeaveFunction;
		typedef TryCall_enter<Destination, void> EnterFunction;
		typedef TryCall_apply<DO(GetValue<TransitionList, Transition<Origin, Event, Destination> >), Event, void> TransitionFunction;
		static const int OriginID = ValueIdentity<DO(GetValue<StateList, Origin>)>::value;
		static const int DestinationID = ValueIdentity<DO(GetValue<StateList, Destination>)>::value;
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
		}
	};
	
// ---------------------------------------------------------------------------
	
	template<typename T>
	struct ConstructFSMLineage : public Car<T>::Type, public ConstructFSMLineage<DO(Cdr<T>)>
	{
		using DO(Car<T>)::process;
		using ConstructFSMLineage<DO(Cdr<T>)>::process;
	};
	
	template<typename T>
	struct ConstructFSMLineage<Cons<T, Nil> > : public T
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

		typedef DO(Apply<Transform, List>) Type;
	};
	
// ---------------------------------------------------------------------------
	
	template<int ID, typename I, typename D, typename Lineage>
	struct FSM : public Lineage
	{
		using Lineage::process;
		typedef TryCall_enter<I, D> EnterFunction;
	
		FSM(D d)
		{
			FSMBase<D>::set_state(ID);
			FSMBase<D>::set_data(d);
			EnterFunction::call(FSMBase<D>::get_data());
		}
	};
	
	template<int ID, typename I, typename Lineage>
	struct FSM<ID, I, void, Lineage> : public Lineage
	{
		using Lineage::process;
		typedef TryCall_enter<I, void> EnterFunction;
	
		FSM( )
		{
			FSMBase<void>::set_state(ID);
			EnterFunction::call();
		}
	};
	
	template<typename I, typename D, typename T>
	struct FSMMaker
	{
		typedef I InitialState;
		typedef D Data;
		typedef DO(CreateStateList<T>) StateList;
		typedef DO(CreateTransitionDependencyList<T>) Transitions;
		typedef DO(CreateTransitionMap<T>) TransitionMap;
		static const int InitialID = ValueIdentity<DO(GetValue<StateList, InitialState>)>::value;
	
		template<typename TT>
		struct CreateTransitionTree
		{
			typedef TransImpl<DO(Car<TT>), Data, DO(Cadr<TT>), StateList, TransitionMap> Type;
		};
	
		typedef ConstructFSMLineage<DO(Apply<CreateTransitionTree, Transitions>)> Lineage;
	
		typedef FSM<InitialID, InitialState, Data, Lineage> Type;
	};
}

#endif

