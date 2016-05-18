#ifndef HAW_SERIAL_CONNECTION_H
#define HAW_SERIAL_CONNECTION_H

#include <string>

#include "lib/Data.h"
#include "lib/concurrent/Lock.hpp"

namespace hw
{
	
/*! \class Connection
 *  \brief Handles Connections over the serial Interface.
 * Can read and write Data to the serial Ports.
 * The Data is packed into a Data_ptr with a variable length.
 * Data_ptr consist of length and data.
 *
 */
	class Connection : public lib::LockableObject<Connection>
	{
		typedef lib::LockableObject<Connection> Super;
		typedef Super::Lock Lock;

		public:
			//! Constructor.
			/*!
				Selects Port and if the port starts as active or passive.
				\param d Serial Ports
				\param a Active or Passive Bool
			*/
			Connection(const std::string& d, bool a);
			//! Destructor.
			/*!
				For safely closing the serial connection.
			*/
			~Connection( );
			//! Is called by the destructor.
			/*!
				Safely closes the serial connection threads and joins them.
				Also sets the running state to false.
			*/
			void close( );
			//! States if the serial connection is connected.
			/*!
				Returns connected status.
			*/
			bool connected( ) const;
			//! States if the serial connection is running.
			/*!
				Returns running status.
			*/
			bool running( ) const;
			//! States if the serial connection is done writing to the serial port.
			/*!
				Returns done writing status.
			*/
			bool doneWriting( ) const;
			//! Sends data over the serial connection.
			/*!
				Writes the data in the serial port send buffer.
				/params lib::Data_ptr Data to send.
			*/
			void sendData(lib::Data_ptr);
			//! Receives data over the serial connection.
			/*!
				Reads data from the receive data buffer.
			*/
			lib::Data_ptr receiveData( );
			//! State of the receive data buffer.
			/*!
				Returns if the receive data buffer has data or not.
			*/
			bool hasData( ) const;
	
		private:
			class Impl;
			Impl *impl_;
	};
}

#endif

