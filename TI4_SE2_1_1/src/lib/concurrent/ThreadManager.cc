#include "lib/concurrent/ThreadManagerImpl.h"

namespace lib
{

ThreadManagerImpl::ThreadManagerImpl(void) : nid_(0)
{
	addThread(pthread_self());
}

ThreadManagerImpl::tid_t ThreadManagerImpl::addThread(pthread_t pid)
{
	Lock guard(this);

	if(ids_.find(pid) != ids_.end())
		throw std::runtime_error("tried to add thread twice!");

	return ids_[pid] = nid_++;
}

void ThreadManagerImpl::removeThread(pthread_t pid)
{
	Lock guard(this);

	std::map<pthread_t, tid_t>::iterator i = ids_.find(pid);

	if(i == ids_.end())
		throw std::runtime_error("tried to remove non-existing thread!");
	
	ids_.erase(i);
}

ThreadManagerImpl::tid_t ThreadManagerImpl::getThread(pthread_t pid)
{
	Lock guard(this);

	std::map<pthread_t, tid_t>::iterator i = ids_.find(pid);

	if(i == ids_.end())
		throw std::runtime_error("tried to access non-existing thread!");
	
	return i->second;
}

}

