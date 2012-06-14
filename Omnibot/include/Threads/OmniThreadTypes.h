#ifndef _OMNITHREADTYPES_H_
#define _OMNITHREADTYPES_H_

enum OmniThreadStatus
{
	IDLE,
	BUSY,
};
enum OmniThreadMode
{
	//stop after eacch task
	OMNITHREAD_DISPATCHED,
	//run until queue is empty
	OMNITHREAD_WORKER
};
#endif
