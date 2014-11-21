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


/**
 * This is class is the top level class in Omnibot
 */
class omnibot: public ircInterfaceClient{
	
	private:
		ircInterface* _irc;                   /**< A pointer to the minibot irc interface*/
		NickManager _nicks;                   /**< A pointer to the nick manger */
		PluginManager _manager;
		bool _passedIrc;		      /**< A flag indicating whether or not _irc 
							needs to be deleted in the destructor*/

		std::vector<OmniCommand*>* _commands; /**< This vector contains the omnibot command 
							objects responsible for processing commands
							directed at the bot proper*/
		
	public:
		omnibot();
		omnibot(ircInterface*);
		virtual ~omnibot();
		void alertEvent(ircEvent&);
		void alertMessage(ircMessage&);
		int connect();
		void exec();

	private:
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
