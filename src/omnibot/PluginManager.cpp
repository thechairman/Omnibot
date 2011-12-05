#include "PluginManager.h"

PluginManager::PluginManager(ircInterface& irc, NickManager& nicks){

	_utils = new PluginUtils(irc, *this, nicks);
	_factory = PluginFactory::instance();

}

PluginManager::~PluginManager(){
	delete _utils;

	std::vector<OmniPlugin*>::iterator iter;
	for(iter = _plugins.begin(); iter != _plugins.end(); iter++){
		(*iter)->wrapUp();
		delete (*iter);
	}

	//TODO not exactly sure what to do with that factory class...
}

bool PluginManager::load(std::string pluginName){
	OmniPlugin* newPlugin = _factory->load(pluginName);

	if(newPlugin == NULL)
	{
		return false;
	}

	//TODO look up permissions for this plugin and set them 
	//in the this plugin utils struct
	PluginUtils* utils= new PluginUtils(*_utils);

	newPlugin->init(utils);

	_plugins.push_back(newPlugin);

	return true;

}

bool PluginManager::unload(std::string pluginName){
	std::vector<OmniPlugin*>::iterator iter;
	for(iter = _plugins.begin(); iter != _plugins.end(); iter++)
	{
		if(!((*iter)->name().compare(pluginName)))
		{
			(*iter)->wrapUp();
			delete (*iter);
			_plugins.erase(iter);
			break;
		}
	}

	//if there is a way to tell the os "hey I'm done with this 
	//dynamic library" we should tell the factory to do that here
	

	return true;
}

void PluginManager::pushMessage(ircMessage msg){
	std::vector<OmniPlugin*>::iterator iter;
	for(iter = _plugins.begin(); iter != _plugins.end(); iter++)
	{
		OmniPlugin::passMessage((*iter), &msg);
	}
}

