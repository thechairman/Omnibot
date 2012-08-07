#ifndef _OMNICOMMSIGNALQUEUE_H_
#define _OMNICOMMSIGNALQUEUE_H_
#include "OmniCommMessage.h"

class OmniCommSignalQueue{
	public:
	virtual void enqueue(OmniCommMessage) =0;
	virtual OmniCommMessage dequeue()=0;
	virtual bool isEmpty()=0;
	virtual unsigned int size()=0;
	static OmniCommSignalQueue* create();

	protected:
	virtual void refIncrease() =0;
	virtual void refDecrease()=0;
	virtual bool refsExist()= 0;

	friend class OmniCommChannel;
	friend class OmniCommChannelManager;
};
#endif
