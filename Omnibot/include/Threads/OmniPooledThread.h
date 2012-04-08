#ifndef _H_OMNIPOOLEDTHREAD_
#define _H_OMNIPOOLEDTHREAD_

#include "OmniThreadedClass.h"
#include "OmniThread.h"

class OmniThread;

class OmniPooledThread
{
 	public:

		virtual int start() = 0;
		virtual int stop() = 0;
		virtual int join() = 0;
		virtual int addTask(OmniThreadedClass*, OmniThreadedClass::Mode) = 0;
		virtual bool isHeld() = 0;
		virtual int id() = 0;
		virtual OmniThread::ThreadStatus status() = 0;
};

#endif
