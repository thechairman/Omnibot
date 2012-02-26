#ifndef _OMNIBOT_
#define _OMNIBOT_

#include "NickManager.h"
#include "PluginManager.h"
#include "ircInterface.h"
#include "ircInterfaceClient.h"

class omnibot: public ircInterfaceClient{
	public:
//		omnibot();
		omnibot(ircInterface*);
		void alertEvent(ircEvent&);
		void alertMessage(ircMessage&);

	private:
		ircInterface* _irc;
		NickManager _nicks;
		PluginManager _manager;
		
	//probably a ton of methods here about what to do
	//with various things to do when certain messages 
	//are recieved
	void join(std::string);
	void part(std::string);
	bool loadPlugin(std::string);
	void dropPlugin(std::string);
	bool isOmniOp(ircUser*);
	bool isRegistered(ircUser*);

};

#endif
