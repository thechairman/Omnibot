#ifndef _IRC_INTERFACE_CLIENT_
#define _IRC_INTERFACE_CLIENT_
#include"ircNotifyClasses.h"

class ircInterfaceClient{
	public:
		virtual void alertEvent(ircEvent& e) = 0;
		virtual void alertMessage(ircMessage& m) = 0;
};
#endif
