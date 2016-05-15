#ifndef HAW_RUN_PUK_H
#define HAW_RUN_PUK_H

#include <stdint.h>

namespace haw
{
	class Puk
	{
		static const uint32_t TYPE_BITMASK = 3;

		public:
		struct Type
		{
			static const uint32_t UNKNOWN = 0;
			static const uint32_t FLAT = 1;
			static const uint32_t LARGE = 2;
		};
		static const uint32_t UPSIDE_DOWN = 4;
		static const uint32_t IS_METAL = 8;

		public:
			Puk(uint32_t w) : width_(w), puk_(Type::UNKNOWN), pos_(0) { }
			uint32_t type( ) const { return puk_ & TYPE_BITMASK; }
			bool upsideDown( ) const { return puk_ & UPSIDE_DOWN; }
			bool metal( ) const { return puk_ & IS_METAL; }
			void update(uint32_t p) { pos_ += p; }
		private:
			uint32_t width_, puk_;
			int32_t pos_;
	};
}

#endif

