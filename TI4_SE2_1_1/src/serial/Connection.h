#ifndef HAW_SERIAL_CONNECTION_H
#define HAW_SERIAL_CONNECTION_H

#include <string>

#include "lib/Data.h"
#include "lib/concurrent/Lock.hpp"

namespace hw
{
	class Connection : public lib::LockableObject<Connection>
	{
		typedef lib::LockableObject<Connection> Super;
		typedef Super::Lock Lock;

		public:
			Connection(const std::string& d, bool a);
			~Connection( );
			void close( );
			bool connected( ) const;
			bool running( ) const;
			bool doneWriting( ) const;
			void sendData(lib::Data_ptr);
			lib::Data_ptr receiveData( );
			bool hasData( ) const;
	
		private:
			class Impl;
			Impl *impl_;
	};
}

#endif

