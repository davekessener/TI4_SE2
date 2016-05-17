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
			Puk(uint32_t id, int32_t w) : id_(id), width_(w), puk_(Type::UNKNOWN), pos_(-w) { }
			uint32_t id( ) const { return id_; }
			uint32_t type( ) const { return puk_ & TYPE_BITMASK; }
			bool upsideDown( ) const { return puk_ & UPSIDE_DOWN; }
			bool metal( ) const { return puk_ & IS_METAL; }
			void update(int32_t p) { pos_ += p; }
			void setType(uint32_t t) { puk_ = t; }
			void setUpsideDown( ) { puk_ |= UPSIDE_DOWN; }
			void setMetal( ) { puk_ |= IS_METAL; }
			int32_t position( ) const { return pos_; }
			bool isIn(int32_t p, int f) const { return pos_ > (p - width_ * (10 - f) / 10) && pos_ < (p - f * width_ / 10); }
		private:
			uint32_t id_;
			int32_t width_, puk_;
			int32_t pos_;
	};

	typedef lib::SmartPtr<Puk> Puk_ptr;
}

#endif

