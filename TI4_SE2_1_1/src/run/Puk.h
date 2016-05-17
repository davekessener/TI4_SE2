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
			Puk(int32_t w) : width_(w), puk_(Type::UNKNOWN), pos_(-w * 11 / 10) { }
			uint32_t type( ) const { return puk_ & TYPE_BITMASK; }
			bool upsideDown( ) const { return puk_ & UPSIDE_DOWN; }
			bool metal( ) const { return puk_ & IS_METAL; }
			void update(int32_t p) { pos_ += p; }
			void setType(uint32_t t) { puk_ = t; }
			void setUpsideDown( ) { puk_ |= UPSIDE_DOWN; }
			void setMetal( ) { puk_ |= IS_METAL; }
			int32_t position( ) const { return pos_; }
			bool isIn(int32_t p) const { return pos_ > (p - 3 * width_ / 4) && pos_ < (p - width_ / 4); }
		private:
			int32_t width_, puk_;
			int32_t pos_;
	};

	typedef lib::SmartPtr<Puk> Puk_ptr;
}

#endif

