#include <stddef.h>
#include <errno.h>
#include <sys/neutrino.h>

#include "lib/qnx/Channel.h"
#include "lib/macro.h"

#ifndef SETIOV
#	define SETIOV(iov, addr, len) \
              ((iov)->iov_base = (void *)(addr), \
               (iov)->iov_len = (len))
#endif

namespace lib { namespace qnx {

namespace
{
	struct qnx_msg_header
	{
		size_t size;
	};
}

Data_ptr Receiver::receive(void)
{
	Data_ptr p;
	qnx_msg_header h;

	while(!static_cast<bool>(p))
	{
		int mid = MsgReceive(ch_->chid_, &h, sizeof(h), NULL);
	
		if(mid < 0)
			return p;
	
		uint8_t *buf = new uint8_t[h.size];
	
		if(MsgRead(mid, buf, h.size, sizeof(h)) < 0)
			MXT_TODO_ERROR; //TODO
	
		p = Data::move(buf, h.size);
	
		if(MsgReply(mid, EOK, NULL, 0) < 0)
			MXT_TODO_ERROR; //TODO
	}

	return p;
}

Connection::~Connection(void)
{
	if(ConnectDetach(coid_) < 0)
		MXT_TODO_ERROR; //TODO
}

void Connection::send(Data_ptr p) const
{
	qnx_msg_header h;
	iov_t iov[2];

	h.size = p->size();
	
	SETIOV(&iov[0], &h, sizeof(h));
	SETIOV(&iov[1], p->data(), p->size());

	if(MsgSendvs(coid_, iov, 2, NULL, 0) < 0)
		MXT_TODO_ERROR; //TODO
}

void Channel::close(void)
{
	if(chid_ > 0)
	{
		if(ChannelDestroy(chid_) < 0)
			MXT_TODO_ERROR; //TODO
		chid_ = 0;
	}
}

Receiver Channel::open(int f)
{
	if(chid_ > 0)
		MXT_TODO_ERROR; //TODO

	if((chid_ = ChannelCreate(f)) < 0)
		MXT_TODO_ERROR; //TODO

	return Receiver(this);
}

Connection Channel::connect(int f) const
{
	int coid = ConnectAttach(0, 0, chid_, _NTO_SIDE_CHANNEL, f);

	if(coid < 0)
		MXT_TODO_ERROR; //TODO
	
	return Connection(coid);
}

} }

