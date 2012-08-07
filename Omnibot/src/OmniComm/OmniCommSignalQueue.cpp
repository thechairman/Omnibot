#include "OmniCommSignalQueue.h"
#include "LinuxOmniCommSignalQueue.h"

OmniCommSignalQueue* OmniCommSignalQueue::create()
{
	return (OmniCommSignalQueue*) new LinuxOmniCommSignalQueue();
}
