#ifndef _H_OMNITHREADPOOL_
#define _H_OMNITHREADPOOL_
#include "OmniPooledThread.h"

class OmniThreadPool
{
	public:
	virtual OmniPooledThread* requestThread() = 0;
	virtual void releaseThread(OmniPooledThread*) = 0;

	static OmniThreadPool& instance();

	protected:
	OmniThreadPool();

	private:
	static OmniThreadPool* _instance;


};
#endif
