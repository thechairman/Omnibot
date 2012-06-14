#include "OmniCommChannel.h"

void OmniCommChannel::sndMessage(OmniCommMessage msg)
{
	_outbound.enqueue(msg);
}
OmniCommMessage OmniCommChannel::rcvMessage()
{
	return _inbound.dequeue();
}
unsigned int OmniCommChannel::pollChannel()
{
	return _inbound.size();
}
void OmniCommChannel::close()
{
	//should send a close notification here
	_inbound.refDecrease();
	_outbound.refDecrease();
}

OmniCommChannel::OmniCommChannel(OmniCommSignalQueue& inbound_, OmniCommSignalQueue& outbound_):
	_inbound(inbound_),_outbound(outbound_),_closed(false)
{
	_inbound.refIncrease();
	_outbound.refIncrease();
}
OmniCommChannel::~OmniCommChannel()
{
	if(!_closed)
	{
		close();
	}
}
