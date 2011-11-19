#ifndef _PLUGIN_FACTORY_
#define _PLUGIN_FACTORY_

#include "OmniPluginInterface.h"

class PluginFactory{

	Public:
	static PluginFactory* instance();
	OmniPluginInterface* load(std::string);
	virtual ~PluginFactory();

	protected:
	PluginFactory();

	private:
	static PluginFactory* _instance = NULL;

};

#endif

