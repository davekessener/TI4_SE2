#ifndef HAW_LIB_QNX_CHANNEL_H
#define HAW_LIB_QNX_CHANNEL_H

#include <utility>
#include <map>

#include "lib/Data.h"
#include "lib/mpl/FtorWrapper.hpp"

namespace lib
{
	namespace qnx
	{
		class Channel;

		class Receiver
		{
			public:
				Data_ptr receive( );
				uint32_t getPulse( );
			private:
				Receiver(Channel *c) : ch_(c) { }
			private:
				Channel *ch_;

				friend class Channel;
		};

		class Connection
		{
			typedef std::map<int, std::pair<Connection *, int> > irs_lookup_t;

			public:
				typedef OneParamFtor<uint32_t, void> isr_fn;
				struct ISR { int id; isr_fn isr; void *area; };

			public:
				Connection( ) : coid_(-1) { }
				~Connection( );
				void send(Data_ptr) const;
				void registerISR(int, isr_fn);
				void close( );
				bool open( ) const { return coid_ >= 0; }
				static std::pair<int, uint32_t> handleISR(int);
			private:
				Connection(int c) : coid_(c) { }
			private:
				int coid_;
				std::map<int, ISR> isrs_;
				static irs_lookup_t lookup_;

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

