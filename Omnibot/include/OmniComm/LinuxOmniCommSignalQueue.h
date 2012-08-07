#ifndef _LINUX_OMNICOMMSIGNALQUEUE_H_
#define _LINUX_OMNICOMMSIGNALQUEUE_H_
#include <deque>
#include <semaphore.h>

#include "OmniCommMessage.h"



class LinuxOmniCommSignalQueue{
	public:
	void enqueue(OmniCommMessage);
	OmniCommMessage dequeue();
	bool isEmpty();
	unsigned int size();

	protected:
	LinuxOmniCommSignalQueue();
	~LinuxOmniCommSignalQueue();
	void refIncrese();
	void refDecrease();
	bool refsExist();


	std::deque<OmniCommMessage> _queue;
	
	sem_t _mutex;	//mutex on the queue
	sem_t _signal; 	//for producer consumer signal;

	friend class OmniCommSignalQueue;
};

#endif
