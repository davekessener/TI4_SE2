#ifndef HAW_LIGHTSCONTROLLER_H
#define HAW_LIGHTSCONTROLLER_H

#include <cstdint>
#include <RingBuffer.hpp>

#define MXT_MSGCOUNT 10

namespace SE
{
	class LightsController
	{
		typedef std::pair<bool, std::uint16_t> value_type;
		typedef lib::RingBuffer<value_type, MXT_MSGCOUNT, lib::RingBufferConcurrency::MultiThreaded> buf_t;

		public:
			static LightsController& instance( );

			void turnOn(std::uint16_t) const;
			void turnOff(std::uint16_t) const;

			void stop( ) { running_ = false; buf_.enqueue(value_type(false, 0)); }
			bool running( ) const { return running_; }

			static void light_controller_thread( );

		private:
			buf_t buf_;
			volatile bool running_;
		private:
			LightsController( );
			~LightsController( );
			LightsController(const LightsController&);
			LightsController& operator=(const LightsController&);
	};
}

#endif

