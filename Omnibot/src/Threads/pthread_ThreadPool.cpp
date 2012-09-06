
//#include <string>

#include "pthread_ThreadPool.h"
#include "OmniPooledThread.h"

#include "ircLog.h"

const std::string FILENAME  = "pthread_ThreadPool.cpp";

OmniPooledThread* pthread_ThreadPool::requestThread()
{
	pthread_PooledThread* tmp = NULL;
	for(Pool_iter i = _threadPool.begin(); i != _threadPool.end(); i++)
	{
		if(! ((*i)->isHeld()))
		{
			tmp =*i;
		}
	}

	if(tmp == NULL)
	{
		addThread(tmp);
	}

	return tmp;

}

void pthread_ThreadPool::releaseThread(OmniPooledThread* tmp)
{	
	pthread_PooledThread* thread = (pthread_PooledThread*) tmp;
	for(Pool_iter i = _threadPool.begin(); i != _threadPool.end(); i++)
	{
		if((*i)->id() == thread->id())
		{
			//release the thread and return;
		}
	}

}


int pthread_ThreadPool::addThread(pthread_PooledThread*& thread){
	thread = new pthread_PooledThread();
	if (!thread->initThread()){
		delete thread;
		thread = NULL;
		return 0;
	}
	_threadPool.push_back(thread);
	ircLog::instance()->logf(FILENAME, "Created new thread with id %d", thread->id());

	return 1;
}

