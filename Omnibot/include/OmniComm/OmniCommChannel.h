#ifndef _OMNICOMMCHANNEL_H_
#define _OMNICOMMCHANNEL_H_
#include "OmniCommSignalQueue.h"

class OmniCommChannel
{
	friend class OmniCommChannelManager;

	public:
		void sndMessage(OmniCommMessage);
		OmniCommMessage rcvMessage();
		unsigned int pollChannel();
		void close();
	protected:
		OmniCommChannel(OmniCommSignalQueue&, OmniCommSignalQueue&);
		~OmniCommChannel();
	
	private:
		OmniCommSignalQueue& _inbound;
		OmniCommSignalQueue& _outbound;
		bool _closed;

};
#endif
