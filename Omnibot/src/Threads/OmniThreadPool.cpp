#include "OmniThreadPool.h"

#include "pthread_ThreadPool.h"

#include <cstddef>

OmniThreadPool::OmniThreadPool(){}

OmniThreadPool* OmniThreadPool::_instance = NULL;

OmniThreadPool& OmniThreadPool::instance(){
	if(_instance == NULL){
		_instance = (OmniThreadPool*) new pthread_ThreadPool();
	}

	return *_instance;
}

