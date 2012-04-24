#include "OmniMutex.h"

#include "pthread_Mutex.h"

OmniMutex* OmniMutex::create()
{
	return new pthread_Mutex;
}
