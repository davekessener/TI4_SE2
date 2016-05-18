#ifndef HAW_TEST_SENSOR_H
#define HAW_TEST_SENSOR_H

#include "hw/LED.h"
#include "lib/TimeP.h"

namespace test
{
	void sensors( );

	using hw::LEDs;
	using hw::LED;
	using lib::Time;
	Time delay = Time::s(3);

	enum //< States
	{
		EnterST_LB_01, 	 //< Einlauf Werkstück
		LeaveST_LB_01,
        EnterST_LB_02,   //< Werkstück in Höhenmessung
        LeaveST_LB_02,
		EnterST_LB_03, 	 //< Werkstück in Weiche
        LeaveST_LB_03,
		EnterST_LB_04,   //< Rutsche voll
        LeaveST_LB_04,
		EnterST_LB_05,   //< Auslauf Werkstück
        LeaveST_LB_05,
		EnterST_HM, 	 //< Höhenmessung
        LeaveST_HM,
		EnterST_MD, 	 //< Metalldetektor
        LeaveST_MD,
		EnterST_GO, 	 //< Weiche offen
        LeaveST_GO,
		EnterST_START, 	 //< Start Knopf
        LeaveST_START,
		EnterST_RESET, 	 //< Reset Knopf
        LeaveST_RESET,
		EnterST_ESTOP,   //< ESTOP, Knopf
        LeaveST_ESTOP,
        EnterST_FINISH,   //< FINISH
        
        EnterError,      //< Error
        LeaveError,

		//Transistions
		TRANS_LB_01,
		TRANS_LB_02,
		TRANS_LB_03,
		TRANS_LB_04,
		TRANS_LB_05,
		TRANS_HM,
		TRANS_MD,
		TRANS_GO,
		TRANS_START,
		TRANS_RESET,
		TRANS_ESTOP,
		TRANS_ERROR,
        TRANS_FINISH

	};
    
//------ Data Creation

    class Store
	{
		public:
			Store( );
			bool check(int);
			std::string msg( ) const;
			void setNextEvent(const std::string& e) { e_ = e; }
		private:
			//std::deque<int> trace_;
			std::string e_;
			//int c_;
	};

	Store::Store( )
	{
/*		trace_.push_back(enterA);

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
		trace_.push_back(enterA);*/

		//c_ = trace_.size();
	}

	bool Store::check(int e)
	{
		//bool r = trace_.front() == e;
		//trace_.pop_front();
		//return r;

		return true;
	}

	/*std::string Store::msg(void) const
	{
		//std::stringstream ss;

		//ss << "upon processing event " << e_ << ", element #" << c_ - trace_.size() << " failed";

		//return ss.str();
	}*/
	
	typedef Store *pStore;
    
	//---- Transistion Events

			    struct ST_LB_01 	//< Einlauf Werkstueck
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};

			    struct ST_LB_02 	//< Werkst?Ek in Höhenmessung
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_LB_03 	//< Werkst?Ek in Weiche
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_LB_04 	//< Rutsche voll
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_LB_05 	//< Auslauf Werkst?Ek
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_HM 		// <Höhenmessung
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_MD 		//< Metalldetektor
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_GO 		//< Weiche offen
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_START 	//< Start Knopf
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_RESET 	//< Reset Knopf
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};
			    struct ST_ESTOP		//< ESTOP
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};

			    struct ST_FINISH		//< ESTOP
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOff(LED::GREEN);
					}

					static void leave(pStore s)
					{
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};

			    struct ERROR 		//< Q2, Knopf
			    {
					static void enter(pStore s)
					{
						LEDs::instance().turnOn(LED::RED);
					}

					static void leave(pStore s)
					{
			            LEDs::instance().turnOff(LED::RED);
						LEDs::instance().turnOn(LED::GREEN);
			            delay.wait();
					}
				};

// # ---------------------------------------------------------------------------
	
	struct ELB_01 { };
	struct ELB_02 { };
    struct ELB_03 { };
    struct ELB_04 { };
    struct ELB_05 { };
    struct EHM { };
    struct EMD { };
    struct EGO { };
    struct ESTART { };
    struct ERESET { };
    struct EESTOP { };

// # ---------------------------------------------------------------------------
	
    /*#define CREATE_RULE(trans) \
        struct Rule_##trans \
        { \
            template<typename E> \
            static void apply(const E& e, pStore s) \
            { \
                //ASSERT_TRUE((s->check(TRANS_##trans)), s->msg());
            } \
        }

	CREATE_RULE(LB_01);
    CREATE_RULE(LB_02);
    CREATE_RULE(LB_03);
    CREATE_RULE(LB_04);
    CREATE_RULE(LB_05);
	CREATE_RULE(HM);
	CREATE_RULE(MD);
    CREATE_RULE(GO);
    CREATE_RULE(START);
    CREATE_RULE(RESET);
    CREATE_RULE(ESTOP);
    CREATE_RULE(ERROR);
    CREATE_RULE(FINISH);*/
    
// # -----------------------------------------------------------------------------
    
    typedef DO(lib::FSMMaker
	<
		ST_LB_01,
		pStore,
		DO(lib::MakeList_4
		<
			lib::Rule<ST_LB_01, ELB_01, ST_HM>,
			lib::Rule<ST_LB_HM, EHM, ST_LB_02>,
			lib::Rule<ST_LB_02, ELB_02, ST_MD>,
            lib::Rule<ST_MD, EMD, ST_LB_03>,
            lib::Rule<ST_LB_03, ELB_03, ST_LB_03>,
            lib::Rule<ST_LB_04, ELB_04, ST_GO>,
            lib::Rule<ST_GO, EGO, ST_LB_05>,
            lib::Rule<ST_LB_05, ELB_05, ST_START>,
            lib::Rule<ST_START, START, ST_RESET>,
            lib::Rule<ST_RESET, RESET, ST_ESTOP>,
            lib::Rule<ST_ESTOP, ESTOP, ST_FINISH>
		>)
	>)
    MyFSM;
    
}

#endif
