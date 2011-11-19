#ifndef _PLUGIN_FACTORY_
#define _PLUGIN_FACTORY_

#include "OmniPluginInterface.h"

class PluginFactory{

	Public:
	OmniPluginInterface* load(std::string);

};

#endif

