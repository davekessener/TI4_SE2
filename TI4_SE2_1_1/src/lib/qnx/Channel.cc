#include <stddef.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <iostream>

#include "lib/qnx/Channel.h"
#include "lib/log/LogManager.h"
#include "hw/HWAccess.h"
#include "lib/macro.h"

#ifndef SETIOV
#	define SETIOV(iov, addr, len) \
              ((iov)->iov_base = (void *)(addr), \
               (iov)->iov_len = (len))
#endif

int gcoid;

namespace lib { namespace qnx {

//namespace
//{
	struct qnx_msg_header
	{
		size_t size;
	};

	const struct sigevent *handle_irs(void *area, int id)
	{
    	struct sigevent* event = (struct sigevent*) area;
		std::pair<int, uint32_t> r = Connection::handleISR(id);

    	event->sigev_notify = SIGEV_PULSE ;
    	event->__sigev_un1.__sigev_coid = r.first;
    	event->__sigev_un2.__st.__sigev_code = 0;
    	event->sigev_value.sival_int = r.second;

    	return event;
	}
//}

Connection::irs_lookup_t Connection::lookup_;

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

uint32_t Receiver::getPulse(void)
{
    struct _pulse pulse;

    lib::log::LogManager::instance().rootLog()->MXT_LOG("started MsgReceivePulse");
	if(MsgReceivePulse(ch_->chid_, &pulse, sizeof(pulse), NULL) < 0)
		MXT_TODO_ERROR; //TODO
    lib::log::LogManager::instance().rootLog()->MXT_LOG("done MsgReceivePulse");

	return pulse.value.sival_int;
}

Connection::~Connection(void)
{
	if(coid_ >= 0)
		close();
}

void Connection::close(void)
{
	if(coid_ < 0)
		MXT_TODO_ERROR; // TODO

	for(std::map<int, ISR>::iterator i1 = isrs_.begin(), i2 = isrs_.end() ; i1 != i2 ; ++i1)
	{
		if(InterruptDetach(i1->second.id) < 0)
			MXT_TODO_ERROR;

		lookup_.erase(lookup_.find(i1->second.id));
	}

	if(ConnectDetach(coid_) < 0)
		MXT_TODO_ERROR; //TODO

	coid_ = -1;
}

void Connection::send(Data_ptr p) const
{
	if(coid_ < 0)
		MXT_TODO_ERROR; // TODO

	qnx_msg_header h;
	iov_t iov[2];

	h.size = p->size();
	
	SETIOV(&iov[0], &h, sizeof(h));
	SETIOV(&iov[1], p->data(), p->size());

	if(MsgSendvs(coid_, iov, 2, NULL, 0) < 0)
		MXT_TODO_ERROR; //TODO
}

void Connection::registerISR(int irq, isr_fn isr)
{
	if(coid_ < 0)
		MXT_TODO_ERROR; //TODO

	if(isrs_.find(irq) != isrs_.end())
		MXT_TODO_ERROR; //TODO
	
	struct sigevent *event = new sigevent;
	lib::log::LogManager::instance().rootLog()->MXT_LOG("SIGEV_PULSE_INIT coid:%i", coid_);
    SIGEV_PULSE_INIT(event, coid_, SIGEV_PULSE_PRIO_INHERIT, 0, 0);

	lib::log::LogManager::instance().rootLog()->MXT_LOG("InterruptAttach");
	int id = InterruptAttach(irq, handle_irs, event, sizeof(*event), 0);
	lib::log::LogManager::instance().rootLog()->MXT_LOG("registered irs with %i", id);

	if(id < 0)
		MXT_TODO_ERROR; //TODO

	isrs_[irq].id = id;
	isrs_[irq].isr = isr;
	isrs_[irq].area = (void *) event;

	lookup_[id] = std::make_pair(this, irq);
}

std::pair<int, uint32_t> Connection::handleISR(int id)
{
	irs_lookup_t::iterator i = lookup_.find(id);

	if(i == lookup_.end())
		MXT_TODO_ERROR; //TODO

	std::pair<Connection *, int> &v = i->second;

	return std::pair<int, uint32_t>(v.first->coid_, v.first->isrs_[v.second].isr());
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

	lib::log::LogManager::instance().rootLog()->MXT_LOG("created channel");

	return Receiver(this);
}

Connection Channel::connect(int f) const
{
	int coid = ConnectAttach(0, 0, chid_, _NTO_SIDE_CHANNEL, f);

	if(coid < 0)
		MXT_TODO_ERROR; //TODO
	
	gcoid = coid;

	lib::log::LogManager::instance().rootLog()->MXT_LOG("created connection %i", coid);

	return Connection(coid);
}

} }

