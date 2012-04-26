#include "pthread_Mutex.h"

pthread_Mutex::pthread_Mutex(){
	if(pthread_mutex_init(&_mutex, NULL)){
		//handle error here
	}

}

pthread_Mutex::~pthread_Mutex(){
	if(pthread_mutex_destroy(&_mutex)){
		//handle error here
	}
		
}

bool pthread_Mutex::lock(){
	if(pthread_mutex_lock(&_mutex))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool pthread_Mutex::unlock(){
	if(pthread_mutex_unlock(&_mutex))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool pthread_Mutex::tryLock(){
	if(pthread_mutex_trylock(&_mutex))
	{
		return false;
	}
	else
	{
		return true;
	}
}
