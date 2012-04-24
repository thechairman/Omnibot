#ifndef _H_PTHREAD_POOLEDTHREAD_
#define _H_PTHREAD_POOLEDTHREAD_

#include <pthread.h>
#include <semaphore.h>
#include <deque>

#include "OmniPooledThread.h"
#include "OmniThreadTypes.h"

class pthread_PooledThread: public OmniPooledThread
{
	public:
		pthread_PooledThread();
		~pthread_PooledThread();
		int start();
		int stop();
		int join();
		int addTask(OmniThreadedClass*, OmniThreadedClass::Mode);
		bool isHeld();
		int id();
		OmniThreadStatus status();
	private:
		void* workFucntion(void*);

		static const unsigned int POLL_INTERVAL = 5000;
		OmniThreadMode _mode;
		bool _started;
		bool _running;
		bool _idle;
		bool _held;
		int _id;

		sem_t _taskSem;
		pthread_t _thread;
		std::deque<std::pair<OmniThreadedClass*, OmniThreadedClass::Mode>* > _tasks;
		static void* workFunction(void*);
		bool initThread();

		friend class pthread_ThreadPool;



};
#endif
