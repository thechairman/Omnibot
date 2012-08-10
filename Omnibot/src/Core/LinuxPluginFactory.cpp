#include <LinuxPluginFactory.h>

#include"hurlbot.h"
#include"bashbot.h"
#include"tomebot.h"

OmniPlugin* LinuxPluginFactory::load(std::string pluginName){
	
	OmniPlugin* plugin = NULL;
	if(!pluginName.compare("hurlbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new hURLbot());
	}
	else if(!pluginName.compare("bashbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new bashbot());
	}
	else if(!pluginName.compare("tomebot")){
		plugin = dynamic_cast<OmniPlugin*>  (new tomebot());
	}

	return plugin;
}

LinuxPluginFactory::~LinuxPluginFactory(){}
LinuxPluginFactory::LinuxPluginFactory(){}
