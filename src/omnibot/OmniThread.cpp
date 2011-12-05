#include<OmniThread.h>

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
	if(threadStarted)
		return THREAD_ALREADY_RUNNING;
	_tasks.push(_data);

	if(pthread_create(&_thread, NULL, OmniThreadCallback, (void*) this) !=0)
		return SYS_THREAD_ERR;

	threadStarted = true;

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

	while(thread->_tasks.size() > 0){

		callBackData data = thread->_tasks.front();

		switch(data._argc){
			case 1:
				data._callback(data._argv[0]);
				break;
			case 2:
				//data._callback(data._argv[0], data._argv[1]);
				break;
		}

		thread->_tasks.pop();
	}


	thread->threadStarted = false;
}

void OmniThread::addTask(void* (*cbFunction)(void*), void* arg){
	callBackData data;
	void** args = new void*[1];
	args[0]=arg;
	data._argv = args;
	data._argc = 1;
	data._callback = cbFunction;

	_tasks.push(data);
}
