#include <sys/neutrino.h>

#include "qnx/Channel.h"

namespace lib { namespace qnx {

namespace
{
	struct qnx_msg_header
	{
		size_t size;
	};
}

void Message::replyOK(Packet_ptr p)
{
	if(mid_ <= 0)
		;// TODO ERROR

	mid_ = -1;
}

Packet_ptr Receiver::receive(void) const
{
	Packet_ptr p(new DataPacket(NULL, 0));
	qnx_msg_header h;

	int mid = MsgReceive(ch_->chid_, &h, sizeof(h), NULL);

	if(mid < 0)
		;// TODO ERROR

	uint8_t *buf = new uint8_t[h.size];

	if(MsgRead(mid, buf, h.size, sizeof(h)) < 0)
		;// TODO ERROR

	static_cast<DataPacket *>(p)->set(buf, h.size);

	if(MsgReply(mid, EOK, NULL, 0) < 0)
		;// TODO ERROR

	return p;
}

Connection::~Connection(void)
{
	if(ConnectDetach(coid_) < 0)
		;// TODO ERROR
}

void Connection::send(Packet_ptr p) const
{
	qnx_msg_header h;
	iov_t iov[2];

	h.size = p->size();
	
	SETIOV(&iov[0], &h, sizeof(h));
	SETIOV(&iov[1], p->data(), p->size());

	if(MsgSendvs(coid_, iov, 2, NULL, 0) < 0)
		;// TODO ERROR
}

Channel::~Channel(void)
{
	if(ChannelDestroy(chid_) < 0)
		;// TODO ERROR
}

Receiver Channel::open(int f)
{
	if(chid_ > 0)
		;// TODO ERROR

	if((chid_ = ChannelCreate(f)) < 0)
		;// TODO ERROR

	return Receiver(this);
}

Connection Channel::connect(int f) const
{
	int coid = ConnectAttach(0, 0, chid_, _NTO_SIDE_CHANNEL, f);

	if(coid < 0)
		;// TODO ERROR
	
	return Connection(coid);
}

} }

