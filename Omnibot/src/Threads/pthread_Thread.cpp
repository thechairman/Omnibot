#include <iostream>

#include "pthread_Thread.h"
#include "OmniThreadPool.h"


pthread_Thread::~pthread_Thread(){
	if(!_clean)
	{
		clean();
	}
}

int pthread_Thread::init()
{
	OmniThreadPool& pool = OmniThreadPool::instance();
	_thread = (pthread_PooledThread*) pool.requestThread();
	_clean = false;
}
int pthread_Thread::start()
{
	_thread->start();
}
int pthread_Thread::stop()
{
	_thread->stop();
}
void pthread_Thread::join()
{

}
void pthread_Thread::setMode(OmniThread::Mode mode)
{
}
void pthread_Thread::addTask(OmniThreadedClass* task, OmniThreadedClass::Mode mode)
{
	std::cout << "adding task to pooled thread" << std::endl;
	_thread->addTask(task, mode);
}
int pthread_Thread::clean()
{
	_thread->stop();
	OmniThreadPool& pool = OmniThreadPool::instance();
	 pool.releaseThread(_thread);
}
