#include "PluginFactory.h"

PluginFactory* PluginFactory::instance(){
	if(_instance == NULL)
	{
		_instance = new PluginFactory();
	}

	return _instance();
}

OmniPluginInterface* load(std::string pluginName){
	//eventually this will load the plugin from a 
	//shared library or dll depending onthe platform
	//for the moment they will be compiled in.
	
	return NULL;


}

PluginFactory::~PluginFactory(){
	if(_instance == NULL)
		return;

	delete _instance;
	
}

PluginFactory::PluginFactory(){}


