#include <LinuxPluginFactory.h>

#include"hurlbot.h"
#include"bashbot.h"
#include"qdbbot.h"
#include"tomebot.h"
#include"8ballbot.h"

OmniPlugin* LinuxPluginFactory::load(std::string pluginName){
	
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
/*	else if(!pluginName.compare("tomebot")){
		plugin = dynamic_cast<OmniPlugin*>  (new tomebot());
	}*/
	else if(!pluginName.compare("8ballbot")){
		plugin = dynamic_cast<OmniPlugin*>  (new eightBallBot());
	}

	return plugin;
}

LinuxPluginFactory::~LinuxPluginFactory(){}
LinuxPluginFactory::LinuxPluginFactory(){}
