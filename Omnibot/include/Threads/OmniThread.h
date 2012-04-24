#ifndef _H_OMNITHREAD_
#define _H_OMNITHREAD_
#include "OmniThreadedClass.h"
#include "OmniThreadTypes.h"

class OmniThread
{
	public:

		virtual int init() = 0;
		virtual int start() = 0;
		virtual int stop() = 0;
		virtual void join() = 0;
		virtual void setMode(OmniThreadMode) = 0;
		virtual void addTask(OmniThreadedClass*, OmniThreadedClass::Mode) = 0;

		static OmniThread* create();


};
#endif
