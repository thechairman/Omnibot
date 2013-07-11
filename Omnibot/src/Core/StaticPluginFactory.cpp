#include <StaticPluginFactory.h>

#include"hurlbot.h"
#include"bashbot.h"
#include"qdbbot.h"
#include"tomebot.h"
#include"8ballbot.h"

OmniPlugin* StaticPluginFactory::load(std::string pluginName){
	
	OmniPlugin* plugin = NULL;
	if(!pluginName.compare("hurlbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new hURLbot());
	}
	else if(!pluginName.compare("bashbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new bashbot());
	}
	else if(!pluginName.compare("qdbbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new qdbbot());
	}
	else if(!pluginName.compare("tomebot")){
		plugin = dynamic_cast<OmniPlugin*>  (new tomebot());
	}
	else if(!pluginName.compare("8ballbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new eightBallBot());
	}

	return plugin;
}

void StaticPluginFactory::unload(std::string pluginName)
{
	return;
}

std::string StaticPluginFactory::listPlugins()
{
	return "hurlbot bashbot qdbot tomebot 8ballbot";
}

StaticPluginFactory::~StaticPluginFactory(){}
StaticPluginFactory::StaticPluginFactory(){}
