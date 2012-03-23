#ifndef _PLUGINUTILS_
#define _PLUGINUTILS_

#include"PluginManager.h"
#include"ircInterface.h"
#include"NickManager.h"
#include"PluginAttrs.h"

//won't compile with out this here
//think it has something to do with omnibot including plugin in manager
//before this in the include chain
//omnibot.cpp->omnibot.h->PluginManager.h->PluginFactory.h->omnipluginInterface.h->thisfile
class PluginManager;

//struct PluginAttrs{};

class PluginUtils {
	public:
		PluginUtils(PluginUtils*, PluginAttrs*);
		PluginUtils(ircInterface*, PluginManager&, NickManager&);
		void sendMessage(std::string channel, std::string message);
		void sendPM(std::string nick, std::string message);
		
		ircUser getUser(std::string);
		ircUser getUser(unsigned int);

		std::vector<ircUser> getChannelUsers(std::string);
		std::vector<ircUser> getAllUsers();
		std::vector<ircUser> getOnlineRegisteredUsers();
		std::vector<ircUser> getAllRegisteredUsers();
		std::vector<ircUser> getAuthenticatedUsers();

	private:
		ircInterface *_irc;
		PluginManager *_manager;
		NickManager *_nicks;
		PluginAttrs* _attrs;
};

#endif
