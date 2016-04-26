#ifndef HAW_LIB_QNX_CHANNEL_H
#define HAW_LIB_QNX_CHANNEL_H

#include "lib/Data.h"

namespace lib
{
	namespace qnx
	{
		class Channel;

		class Receiver
		{
			public:
				Data_ptr receive( ) const;
			private:
				Receiver(Channel *c) : ch_(c) { }
			private:
				Channel *ch_;

				friend class Channel;
		};

		class Connection
		{
			public:
				~Connection( );
				void send(Data_ptr) const;
			private:
				Connection(int c) : coid_(c) { }
			private:
				int coid_;

				friend class Channel;
		};

		class Channel
		{
			public:
				Channel( ) : chid_(0) { }
				~Channel( ) { close(); }
				Receiver open(int = 0);
				Connection connect(int = 0) const;
				bool isOpen( ) const { return chid_ > 0; }
				void close( );
			private:
				int chid_;

				friend class Receiver;
		};
	}
}

#endif

