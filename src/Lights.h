#ifndef HAW_LIGHTS_H
#define HAW_LIGHTS_H

#include <cstdint>

namespace SE
{
	class Lights
	{
		public:
			static Lights& instance( );

			void turnOn(std::uint16_t) const;
			void turnOff(std::uint16_t) const;

			static const std::uint16_t GREEN  = 0x20 // 0010 0000
			static const std::uint16_t YELLOW = 0x40 // 0100 0000
			static const std::uint16_t RED    = 0x80 // 1000 0000

		private:
			Lights( ) { }
			~Lights( ) { }
			Lights(const Lights&);
			Lights& operator=(const Lights&);
	};
}

#endif

