#ifndef _H_OMNITHREAD_
#define _H_OMNITHREAD_
#include "OmniThreadedClass.h"



class OmniThread
{
	public:

		enum ThreadStatus
		{
			IDLE,
			BUSY,
		};
		enum Mode
		{
			//stop after eacch task
			OMNITHREAD_DISPATCHED,
			//run until queue is empty
			OMNITHREAD_WORKER
		};
		virtual int init() = 0;
		virtual int start() = 0;
		virtual int stop() = 0;
		virtual void join() = 0;
		virtual void setMode(OmniThread::Mode) = 0;
		virtual void addTask(OmniThreadedClass*, OmniThreadedClass::Mode) = 0;


};
#endif
