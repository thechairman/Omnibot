#ifndef _OMNICOMMQUEUEMANAGER_H_
#define _OMNICOMMQUEUEMANAGER_H_

#include <vector>
#include <utility>
#include "OmniCommSignalQueue.h"
#include "OmniCommChannel.h"

class OmniCommChannelManager
{
	public:
		static const int QUEUES_PER_ENDPOINT = 2;
		static const int ENDPOINTS_RETURNED = 2;
		std::pair<OmniCommSignalQueue*, OmniCommSignalQueue*> getNewQueues();
		std::pair<OmniCommChannel*, OmniCommChannel*> getNewEndpoints();
		void cullFreedQueues();

	private:
		std::vector<OmniCommSignalQueue*> _queues;

};

#endif
