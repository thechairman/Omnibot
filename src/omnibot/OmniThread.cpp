#include<OmniThread.h>

#include<iostream>
OmniThread::OmniThread():threadStarted(false){}

void OmniThread::callBack(void* (*function)(void*)){
	_data._callback = function;
}

void OmniThread::arg(void* arg_){

	//check that arc matchtes here
	void** args = new void*[1];
	args[0]=arg_;
	_data._argv = args;
	_data._argc = 1;
} 

int OmniThread::start(){
	std::cout<< "OmniThread: starting thread" << std::endl;
	if(threadStarted)
		return THREAD_ALREADY_RUNNING;
	_tasks.push(_data);

	return createThread();
}
int OmniThread::createThread(){
	std::cout<< "OmniThread: creating pthread" << std::endl;
	_stateMutex.lock();
	if(pthread_create(&_thread, NULL, OmniThreadCallback, (void*) this) !=0){
		_stateMutex.unlock();
		return SYS_THREAD_ERR;
	}

	threadStarted = true;
	_stateMutex.unlock();

	return SUCCESS;	
}

void OmniThread::join(){
	pthread_join(_thread, NULL);
	
}

bool OmniThread::isRunning(){

	return threadStarted;
}

void* OmniThread::OmniThreadCallback(void* thread_){
	OmniThread* thread = (OmniThread*) thread_;

	thread->_taskMutex.lock();
	while(thread->_tasks.size() > 0){
		std::cout << "OmniThread: callback: number of task i queue: " << thread->_tasks.size() << std::endl;
		thread->_stateMutex.unlock();
		callBackData data = thread->_tasks.front();
		thread->_taskMutex.unlock();

		std::cout<< "OmniThread: activating task callback" << std::endl;
		switch(data._argc){
			case 1:
				data._callback(data._argv[0]);
				break;
			case 2:
				//data._callback(data._argv[0], data._argv[1]);
				break;
		}
		thread->_taskMutex.lock();
		thread->_stateMutex.lock();
		thread->_tasks.pop();
	
	}

	thread->_taskMutex.unlock();
	thread->threadStarted = false;
	thread->_stateMutex.unlock();
	std::cout <<"OmniThread: callback: thread exiting"<<std::endl;
}

int OmniThread::addTask(void* (*cbFunction)(void*), void* arg){
	callBackData data;
	void** args = new void*[1];
	args[0]=arg;
	data._argv = args;
	data._argc = 1;
	data._callback = cbFunction;
	
	_taskMutex.lock();
	_tasks.push(data);
	std::cout << "OmniThread: Number of strings in queue: " << _tasks.size() << std::endl;
	_taskMutex.unlock();

	_stateMutex.lock();
	if(!threadStarted){
		_stateMutex.unlock();
		std::cout << "OmniThread: had to resatart the thread" << std::endl;
		return createThread();
	}
	_stateMutex.unlock();

	
}


/**************************************************************
 *  OmniMutex funcions
 *  **********************************************************/

OmniMutex::OmniMutex(){
	if(pthread_mutex_init(&_mutex, NULL)){
		//handle error here
	}

}

OmniMutex::~OmniMutex(){
	if(pthread_mutex_destroy(&_mutex)){
		//handle error here
	}
		
}

bool OmniMutex::lock(){
	if(pthread_mutex_lock(&_mutex))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool OmniMutex::unlock(){
	if(pthread_mutex_unlock(&_mutex))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool OmniMutex::tryLock(){
	if(pthread_mutex_trylock(&_mutex))
	{
		return false;
	}
	else
	{
		return true;
	}
}
