#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include <string>
#include "ircNotifyClasses.h"
#include <vector>

class PluginManager{
	public:
		PluginManager(PluginUtils*);
		bool load(std::string);
		bool unload(std::string);
		
		void pushMessage(ircMessage);

	private:
		PluginUtils* _utils;
		std::vector<OmniPlugins*> _plugins;
		PluginFactory* _factory;

};

#endif
