#include <LinuxPluginFactory.h>

#include"hurlbot.h"
#include"bashbot.h"

OmniPlugin* LinuxPluginFactory::load(std::string pluginName){
	
	OmniPlugin* plugin = NULL;
	if(!pluginName.compare("hurlbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new hURLbot());
	}
	else if(!pluginName.compare("bashbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new bashbot());
	}

	return plugin;
}

LinuxPluginFactory::~LinuxPluginFactory(){}
LinuxPluginFactory::LinuxPluginFactory(){}
