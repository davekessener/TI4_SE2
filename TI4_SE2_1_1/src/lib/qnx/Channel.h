#ifndef HAW_LIB_QNX_CHANNEL_H
#define HAW_LIB_QNX_CHANNEL_H

#include "lib/Data.h"
#include "lib/mpl/FtorWrapper.hpp"

namespace lib
{
	namespace qnx
	{
		class Channel;

		class Receiver
		{
			typedef OneParamFtor<void, uint32_t> callback_fn;

			public:
				Data_ptr receive( );
			private:
				Receiver(Channel *c) : ch_(c) { }
				void setPulseHandler(callback_fn f) { ph_ = f; }
			private:
				Channel *ch_;
				callback_fn ph_;

				friend class Channel;
		};

		class Connection
		{
			public:
				~Connection( );
				void send(Data_ptr) const;
				void pulse(uint32_t) const;
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

