#ifndef HAW_HW_SWITCH_H
#define HAW_HW_SWITCH_H

namespace hw
{
	class Actuator;

	class Switch
	{
		public:
			static const uint8_t OPEN = 0;
			static const uint8_t CLOSED = 1;

			void open( ) { setState(OPEN); }
			void close( ) { setState(CLOSED); }
			void setState(uint8_t);
		private:
			Switch( ) { }
			~Switch( ) { }
			Switch(const Switch&);
			Switch& operator=(const Switch&);

			friend class Actuator;
	};
}

#endif

