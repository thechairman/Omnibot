#include "pthread_PooledThread.h"
#include <utility>
#include <iostream>

#include "ircLog.h"

const std::string FILENAME = "pthread_PooledThread.cpp";

pthread_PooledThread::pthread_PooledThread():_started(false),_running(false),_idle(false),_held(false),_id(0),_thread(NULL)
{
	sem_init(&_taskSem, 0, 0);
}

pthread_PooledThread::~pthread_PooledThread()
{
	sem_destroy(&_taskSem);
}

int pthread_PooledThread::start()
{
	_started = true;
	return 0;
}
int pthread_PooledThread::stop()
{
	_started = false;
	return 0;
}
int pthread_PooledThread::join()
{
	return 0;
}
int pthread_PooledThread::addTask(OmniThreadedClass* task, OmniThreadedClass::Mode mode)
{
	//std::cout << "pooledThread: adding task to queue "<< std::endl;
	ircLog::instance()->logf(FILENAME, "thread %d : adding task to queue", (unsigned int) _thread);
	std::pair<OmniThreadedClass*, OmniThreadedClass::Mode>* tmp = 
		new std::pair<OmniThreadedClass*, OmniThreadedClass::Mode>(task, mode);
	
	_tasks.push_back(tmp);
	//std::cout << "pooledThread: task queue has " << _tasks.size() << "tasks" << std::endl;
	ircLog::instance()->logf(FILENAME, "thread %d: task queue has %d tasks", (unsigned int) _thread, _tasks.size());
	sem_post(&_taskSem);
	return 0;

}
bool pthread_PooledThread::isHeld()
{
	return _held;
}
int pthread_PooledThread::id()
{
	return (int) _thread;
}
OmniThreadStatus pthread_PooledThread::status()
{
}
void* pthread_PooledThread::workFunction(void* thread_)
{
	pthread_PooledThread* thread = static_cast<pthread_PooledThread*>(thread_);
	//std::cout << "PooledThread: executing task" << std::endl;
	ircLog::instance()->logf(FILENAME, "thread %d: executing task", (unsigned int)  thread->id());
	while(thread->_running)
	{
		while(thread->_started)
		{
			sem_wait(&(thread->_taskSem));

			//std::cout << "PooledThread: executing next task" << std::endl;
			ircLog::instance()->logf(FILENAME, "thread %d: executing next task", (unsigned int) thread->id());
			std::pair<OmniThreadedClass*, OmniThreadedClass::Mode>* task;
			task = thread->_tasks.front();
			thread->_tasks.pop_front();

			//if ladder here for modes
			//call the appropriate method in the class
			if(task->second == OmniThreadedClass::MODE_A)
			{
				task->first->runA();
			}
		}

	}
}
bool pthread_PooledThread::initThread()
{
	_running = true;
	int failed = pthread_create(&_thread, NULL, workFunction, (void*) this);
	
	if(!failed)
		return true;
	else 
		return false;
}
	
