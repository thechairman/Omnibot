#include "OmniCommChannelManager.h"

std::pair<OmniCommSignalQueue*, OmniCommSignalQueue*> OmniCommChannelManager::getNewQueues()
{
	OmniCommSignalQueue* retvals[QUEUES_PER_ENDPOINT];
       	unsigned int rvi;	//retval index

	for(size_t i = 0, rvi = 0; 
		i < _queues.size() && rvi < QUEUES_PER_ENDPOINT; 
		++i)
	{
		if(!_queues[i]->refsExist())
		{
			retvals[rvi] = _queues[i];
			++rvi;
		}
	}

	while(rvi < QUEUES_PER_ENDPOINT)
	{
		retvals[rvi] = OmniCommSignalQueue::create();
	}

	return std::pair<OmniCommSignalQueue*, OmniCommSignalQueue*>(retvals[0], retvals[1]);
}

 std::pair<OmniCommChannel*, OmniCommChannel*> OmniCommChannelManager::getNewEndpoints()
{
	std::pair<OmniCommSignalQueue*, OmniCommSignalQueue*> queuePair = getNewQueues();
	OmniCommChannel* endpoints[ENDPOINTS_RETURNED];

	endpoints[0] = new OmniCommChannel(*(queuePair.first), *(queuePair.second));
	endpoints[1] = new OmniCommChannel(*(queuePair.second), *(queuePair.first));

	std::pair<OmniCommChannel*, OmniCommChannel*> endpointPair(endpoints[0], endpoints[1]);

	return endpointPair;


}

void OmniCommChannelManager::cullFreedQueues()
{
	std::vector<OmniCommSignalQueue*>::iterator iter;

	//a while instead of a for, because I am not sure
	//the loop optimization would like this...
	iter = _queues.begin();
	while(iter != _queues.end())
	{	
		if(!(*iter)->refsExist())
		{
			_queues.erase(iter);
			iter = _queues.begin();
		}
	}
}
