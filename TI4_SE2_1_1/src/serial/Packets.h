#ifndef HAW_SERIAL_PACKETS_H
#define HAW_SERIAL_PACKETS_H

#include <stdint.h>
#include <memory>

#include <SmartPtr.hpp>

namespace hw
{
	class Packet
	{
		public:
			virtual ~Packet( ) { }
			virtual uint32_t size( ) const = 0;
			virtual const uint8_t *data( ) const = 0;
			virtual uint8_t id( ) const = 0;
			virtual uint32_t hash( ) const = 0;

			static const uint8_t DATA_ID = 0;
			static const uint8_t OK_ID = 1;
			static const uint8_t ERROR_ID = 2;
			static const uint8_t ACHNOWLEDGE_ID = 3;
	};

	typedef lib::SmartPtr<Packet> Packet_ptr;

	class DataPacket : public Packet
	{
		public:
			template<typename T>
			explicit DataPacket(const T& v) { init(&v, sizeof(v)); }
			DataPacket(const void *d, uint32_t s) { init(d, s); }
			~DataPacket( ) { delete[] data_; }

			void set(void *, size_t);

			static Packet_ptr assemble(const uint8_t *d, uint32_t s)
				{ return Packet_ptr(new DataPacket(d, s)); }
			
			uint32_t size( ) const { return size_; }
			const uint8_t *data( ) const { return data_; }
			uint8_t id( ) const { return Packet::DATA_ID; }
			uint32_t hash( ) const { return 0x12345678; }

		private:
			void init(const void *d, uint32_t s);

		private:
			uint8_t *data_;
			uint32_t size_;
	};

	class OKPacket : public Packet
	{
		public:
			OKPacket(uint32_t s) : status_(s) { }

			static Packet_ptr assemble(const uint8_t *d, uint32_t s)
				{ return Packet_ptr(new OKPacket(*((uint32_t *) d))); }

			uint32_t size( ) const { return sizeof(status_); }
			const uint8_t *data( ) const { return (uint8_t *) &status_; }
			uint8_t id( ) const { return Packet::OK_ID; }
			uint32_t hash( ) const { return 0x12345678; }

		private:
			const uint32_t status_;
	};

	class ErrorPacket : public Packet
	{
		public:
			ErrorPacket(uint32_t s) : status_(s) { }

			static Packet_ptr assemble(const uint8_t *d, uint32_t s)
				{ return Packet_ptr(new ErrorPacket(*((uint32_t *) d))); }

			uint32_t size( ) const { return sizeof(status_); }
			const uint8_t *data( ) const { return (uint8_t *) &status_; }
			uint8_t id( ) const { return Packet::ERROR_ID; }
			uint32_t hash( ) const { return 0x12345678; }

		private:
			const uint32_t status_;
	};

	class AcknowledgePacket : public Packet
	{
		public:
			AcknowledgePacket(uint32_t s) : status_(s) { }

			static Packet_ptr assemble(const uint8_t *d, uint32_t s)
				{ return Packet_ptr(new AcknowledgePacket(*((uint32_t *) d))); }

			uint32_t size( ) const { return sizeof(status_); }
			const uint8_t *data( ) const { return (uint8_t *) &status_; }
			uint8_t id( ) const { return Packet::ACKNOWLEDGE_ID; }
			uint32_t hash( ) const { return 0x12345678; }

		private:
			const uint32_t status_;
	};
}

#endif

