#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include <string>
#include "ircNotifyClasses.h"
#include "PluginFactory.h"
#include "OmniPluginInterface.h"
#include <vector>

class PluginManager{
	public:
		PluginManager(ircInterface* irc, NickManager* nicks);
		virtual ~PluginManager();
		bool load(std::string);
		bool unload(std::string);
		
		void pushMessage(ircMessage);

	private:
		PluginUtils* _utils;
		std::vector<OmniPlugin*> _plugins;
		PluginFactory* _factory;

};

#endif
