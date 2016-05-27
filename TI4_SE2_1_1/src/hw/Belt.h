#ifndef HAW_HW_BELT_H
#define HAW_HW_BELT_H

namespace hw
{
	class Actuator;

	class Belt
	{
		public:
			static const uint8_t LEFT = 0x02;
			static const uint8_t RIGHT = 0x01;
			static const uint8_t FAST = 0x00;
			static const uint8_t SLOW = 0x04;
			static const uint8_t PAUSE = 0x08;
			static const uint8_t BITMASK = LEFT | RIGHT | FAST | SLOW | PAUSE;

			void move(uint8_t, uint8_t);
			void changeSpeed(uint8_t);
			void stop( );
			void pause( ) { doPause(true); }
			void unpause( ) { doPause(false); }
			void doPause(bool);
		private:
			Belt( ) { }
			~Belt( ) { }
			Belt(const Belt&);
			Belt& operator=(const Belt&);

			friend class Actuator;
	};
}

#endif

