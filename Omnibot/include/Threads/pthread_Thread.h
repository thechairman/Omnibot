#ifndef _H_PTHREAD_THREAD_
#define _H_PTHREAD_THREAD_

#include "OmniThread.h"
#include "pthread_PooledThread.h"

class pthread_Thread: public OmniThread
{
	public:
		~pthread_Thread();
		int init();
		int start();
		int stop();
		void join();
		void setMode(OmniThread::Mode);
		void addTask(OmniThreadedClass*, OmniThreadedClass::Mode);
		int clean();
		bool isAlive();
		bool isIdle();

	private:
		bool _clean;
		pthread_PooledThread* _thread;

};

#endif
