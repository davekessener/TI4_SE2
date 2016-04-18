#ifndef HAW_LIB_QNX_CHANNEL_H
#define HAW_LIB_QNX_CHANNEL_H

#include <serial/Packets.h>

namespace lib
{
	namespace qnx
	{
		class Channel;

		class Receiver
		{
			public:
				Packet_ptr receive( ) const;
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
				void send(Packet_ptr) const;
			private:
				Connection(int c) : coid_(c) { }
			private:
				int coid_;

				friend class Channel;
		};

		class Channel
		{
			public:
				Channel( ) : chid_(-1) { }
				~Channel( );
				Receiver open(int = 0);
				Connection connect(int = 0) const;
				bool isOpen( ) const { return chid_ > 0; }
			private:
				int chid_;

				friend class Receiver;
		};
	}
}

#endif

