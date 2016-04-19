#ifndef HAW_SERIAL_CONNECTION_H
#define HAW_SERIAL_CONNECTION_H

#include <string>

#include "serial/Packets.h"
#include "lib/concurrent/Thread.h"
#include "lib/RingBuffer.hpp"

#define MXT_PACKETBUFSIZE 256

namespace hw
{
	class Connection
	{
		typedef lib::RingBuffer<Packet_ptr, MXT_PACKETBUFSIZE, lib::RingBufferConcurrency::MultiThreaded> buf_t;

		public:
			Connection(const std::string&, bool);
			~Connection( );
			void write(const void *d, size_t);
			Packet_ptr read( );
			void close( );

		private:
			void thread( );
		private:
			const std::string device_;
			int f_;
			bool active_;
			bool running_;
			std::auto_ptr<lib::Thread> thread_;
			buf_t wbuf_, rbuf_;
	};
}

#endif

