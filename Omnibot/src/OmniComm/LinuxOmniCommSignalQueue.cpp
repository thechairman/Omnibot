#include "LinuxOmniCommSignalQueue.h"

void LinuxOmniCommSignalQueue::enqueue(OmniCommMessage _msg)
{	
	sem_wait(&_mutex);
	_queue.push_back(_msg);
	sem_post(&_mutex);
	sem_post(&_signal);
}
OmniCommMessage LinuxOmniCommSignalQueue::dequeue()
{
	sem_wait(&_signal);
	sem_wait(&_mutex);
	OmniCommMessage temp = _queue.front();
	_queue.pop_front();
	sem_post(&_mutex);
	return temp;
}

bool LinuxOmniCommSignalQueue::isEmpty()
{	
	sem_wait(&_mutex);
	bool cond = _queue.size() > 0;
	sem_post(&_mutex);

	if(cond)
	{
		return false;
	}

	return true;
}
unsigned int LinuxOmniCommSignalQueue::size()
{
	sem_wait(&_mutex);
	unsigned int size = _queue.size(); 
	sem_post(&_mutex);

	return size;
}
LinuxOmniCommSignalQueue::LinuxOmniCommSignalQueue()
{
	//the intial queue is empty so we 
	//want the semaphore to indicate that
	sem_init(&_signal, 0, 0);

	//this em is a mutex so we want it to
	//be free to start
	sem_init(&_mutex, 0, 1);
}
LinuxOmniCommSignalQueue::~LinuxOmniCommSignalQueue()
{
	sem_destroy(&_mutex);
	sem_destroy(&_signal);

}

