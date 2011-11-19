#ifndef _PLUGINUTILS_
#define _PLUGINUTILS_

#include"ircInterface.h"
#include"PluginManger.h"
#include"NickManager.h"

class PluginUtils {
	public:
		PluginUtils(PluginUtils*, PluginAttrs*);
		PluginUtils(ircInterface*, PluginManager*, NickManager*);
		void sendMessage(std::string channel, std::string message);
		void sendPM(std::string nick, std::string message);

	private:
		ircInterface* _irc;
		PluginManager* _manager;
		NickManager* _nicks;
		PluginAttrs* _attrs;
};

#endif
