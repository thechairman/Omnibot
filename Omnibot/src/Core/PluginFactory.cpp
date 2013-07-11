#include "PluginFactory.h"

#include "StaticPluginFactory.h"
#include "LinuxPluginFactory.h"
#include "OmniConfigParser.h"

PluginFactory* PluginFactory::_instance = NULL;

PluginFactory* PluginFactory::instance(){
	if(_instance == NULL)
	{
		OmniConfigParser::pluginLoadModes mode;
		mode = OmniConfigParser::instance()->pluginLoadMode();

		if(mode == OmniConfigParser::PLM_DYNAMIC)
		{
			#ifdef DYNAMIC_LOADING
			_instance = new LinuxPluginFactory();
			#else
			_instance = new StaticPluginFactory();
			#endif

		}
		else //mode == PLM_STATIC
		{
			_instance = new StaticPluginFactory();
		}

	}

	return _instance;
}


PluginFactory::~PluginFactory(){
	if(_instance == NULL)
		return;

	delete _instance;
	
}

PluginFactory::PluginFactory(){}


