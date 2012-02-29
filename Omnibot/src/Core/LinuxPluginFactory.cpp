#include <LinuxPluginFactory.h>

#include"hurlbot.h"

OmniPlugin* LinuxPluginFactory::load(std::string pluginName){
	
	OmniPlugin* plugin = NULL;
	if(!pluginName.compare("hurlbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new hURLbot());
	}

	return plugin;
}

LinuxPluginFactory::~LinuxPluginFactory(){}
LinuxPluginFactory::LinuxPluginFactory(){}
