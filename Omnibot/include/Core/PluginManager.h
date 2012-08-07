#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include <string>
#include <vector>
#include "ircInterface.h"
#include "ircNotifyClasses.h"
#include "PluginFactory.h"
#include "OmniPluginInterface.h"
#include "PluginUtils.h"
#include "NickManager.h"

#include "OmniCommChannelManager.h"

class OmniPlugin;
class PluginUtils;
class PluginFactory;

class PluginManager{
	public:
		PluginManager(ircInterface*, NickManager&);
		virtual ~PluginManager();
		bool load(std::string);
		bool unload(std::string);
		
		void pushMessage(ircMessage& );

		std::string listLoadedPlugins();

		OmniCommChannel* setupChannel(std::string);

	private:
		PluginUtils* _utils;
		std::vector<OmniPlugin*> _plugins;
		PluginFactory* _factory;
		OmniCommChannelManager* _commChannels;


};

#endif
