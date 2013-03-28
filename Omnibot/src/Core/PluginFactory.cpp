#include "PluginFactory.h"

#include "StaticPluginFactory.h"
PluginFactory* PluginFactory::_instance = NULL;

PluginFactory* PluginFactory::instance(){
	if(_instance == NULL)
	{
		_instance = new StaticPluginFactory();
	}

	return _instance;
}


PluginFactory::~PluginFactory(){
	if(_instance == NULL)
		return;

	delete _instance;
	
}

PluginFactory::PluginFactory(){}


