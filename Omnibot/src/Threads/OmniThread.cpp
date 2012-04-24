#include "OmniThread.h"
#include "pthread_Thread.h"


OmniThread* OmniThread::create()
{
	return dynamic_cast<OmniThread*> (new pthread_Thread()); 
}
