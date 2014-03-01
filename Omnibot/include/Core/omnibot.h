#ifndef _OMNIBOT_
#define _OMNIBOT_

#include <vector>

#include "NickManager.h"
#include "PluginManager.h"
//#include "OmniBlocker.h"
#include "ircInterface.h"
#include "ircInterfaceClient.h"

#include "OmniCommand.h"
#include "OmniCommandBuilder.h"

class omnibot: public ircInterfaceClient{
	public:
		omnibot();
		omnibot(ircInterface*);
		virtual ~omnibot();
		void alertEvent(ircEvent&);
		void alertMessage(ircMessage&);
		int connect();
		void exec();

	private:
		ircInterface* _irc;
		NickManager _nicks;
		PluginManager _manager;
		//OmniBlocker* _blocker;
		bool _passedIrc;

		std::vector<OmniCommand*>* _commands;
		
		//probably a ton of methods here about what to do
		//with various things to do when certain messages 
		//are recieved
		void join(std::string);
		void part(std::string);
		bool loadPlugin(std::string);
		void dropPlugin(std::string);
		bool isOmniOp(ircUser&);
		bool isRegistered(ircUser&);
		void createCommands();
		void clearCommands();


};

#endif
