#ifndef HAW_RUN_INITIALIZATION_H
#define HAW_RUN_INITIALIZATION_H

#include "run/State.h"

namespace haw
{
	class Calibrator
	{
		typedef void (Calibrator::*state_fn)(const SensorEvent&);

		class Measurer
		{
			typedef void(Measurer::*state_fn)(const SensorEvent&);

			public:
				Measurer( ) : { reset(); }
				void reset( ) { state_ = &Measurer::start; height_ = -1; }
				void update(const SensorEvent& e) { (this->*state_)(e); }
				void tick( );
				bool done( ) const { return height_ < (uint16_t)-1 && state_ == &Measurer::end; }
				uint16_t getHeight( ) const { return ((uint16_t) -1) - height_; }
			private:
				void start(const SensorEvent&);
				void toHM(const SensorEvent&);
				void inHM(const SensorEvent&);
				void back(const SensorEvent&);
				void end(const SensorEvent&) { }
			private:
				state_fn state_;
				uint16_t height_;
		};

		public:
			Calibrator( ) : state_(&Calibrator::start), cur_(0) { }
			void process(const SensorEvent& e) { (this->*state_)(e); }
			void tick( ) { meas_.tick(); }
		private:
			void measureFlat(const SensorEvent&);
			void measureLarge(const SensorEvent&);
			void start(const SensorEvent&);
			void toHM(const SensorEvent&);
			void toSwitch(const SensorEvent&);
			void toEnd(const SensorEvent&);
		private:
			Measurer meas_;
			lib::Time fast_, slow_;
			state_fn state_;
			int cur_;
	};

	class Initialization : public State
	{
		public:
			Initialization( );
			virtual void enter( );
			virtual void exit( );
//			virtual void update(lib::Time) { }
			virtual void process(const Event&);
			virtual void execute( ) { cali_.tick(); }
		private:
			Calibrator cali_;
	};
}

#endif

