

#ifndef _POSIX_OMNI_BLOCKER_H_
#define _POSIX_OMNI_BLOCKER_H_


#include <semaphore.h>
#include "OmniBlocker.h"

class posix_OmniBlocker:public OmniBlocker
{
	public:
		posix_OmniBlocker();
		~posix_OmniBlocker();
		void block();
		void release();
		unsigned int numberWaiting();
	
	private:
		unsigned int _numBlocked;
		sem_t _blockingSem;
		sem_t _numSem;

};

#endif
