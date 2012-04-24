
#ifndef _PTHREAD_MUTEX_H_
#define _PTHREAD_MUTEX_H_

#include <pthread.h>

#include "OmniMutex.h"
class pthread_Mutex:public OmniMutex{
	public:
		pthread_Mutex();
		~pthread_Mutex();
		bool lock(); 
		bool unlock();
		bool tryLock();

	private:
		pthread_mutex_t _mutex;

};

#endif
