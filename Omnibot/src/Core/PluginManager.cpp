#include <sstream>
#include <iostream>

#include "PluginManager.h"
#include "ircLog.h"

const std::string FILENAME = "PluginManager.cpp";

PluginManager::PluginManager(ircInterface* irc, NickManager& nicks)
{
	_utils = new PluginUtils(irc, *this, nicks);
	_factory = PluginFactory::instance();
	_commChannels = new OmniCommChannelManager();

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

	std::vector<OmniPlugin*>::iterator iter;
	for(iter = _plugins.begin(); iter != _plugins.end(); iter++)
	{
		if(!((*iter)->name().compare(pluginName)))
		{
			//TODO log that plugin was already loaded
			return true;
		}
	}

	OmniPlugin* newPlugin = _factory->load(pluginName);

	if(newPlugin == NULL)
	{
		return false;
	}

	//for now
	static PluginAttrs attrs;

	//TODO look up permissions for this plugin and set them 
	//in the this plugin utils struct
	PluginUtils* utils= new PluginUtils(_utils, &attrs );

	newPlugin->init(utils);

	_plugins.push_back(newPlugin);

	return true;

}

bool PluginManager::unload(std::string pluginName){
	std::vector<OmniPlugin*>::iterator iter;
	bool retval = false;
	for(iter = _plugins.begin(); iter != _plugins.end(); iter++)
	{
		if(!((*iter)->name().compare(pluginName)))
		{
			(*iter)->wrapUp();
			delete (*iter);
			_plugins.erase(iter);
			retval = true;
			break;
		}
	}

	//if there is a way to tell the os "hey I'm done with this 
	//dynamic library" we should tell the factory to do that here
	

	return retval;
}

void PluginManager::pushMessage(ircMessage& msg){
	std::vector<OmniPlugin*>::iterator iter;
	for(iter = _plugins.begin(); iter != _plugins.end(); iter++)
	{
		//std::cout <<"PluginManager: passing message to plugin: "<< (*iter)->name() << std::endl;
		ircLog::instance()->logf(FILENAME, "passing message to plugin: %s", (*iter)->name().c_str());

		OmniPlugin::passMessage((*iter), msg);
	}
}

std::string PluginManager::listLoadedPlugins()
{
	std::vector<OmniPlugin*>::iterator iter;
	std::stringstream list;
	for(iter = _plugins.begin(); iter != _plugins.end(); iter++)
	{	
		list << (*iter)->name() << " ";
	//	std::cout <<"PluginManager: passing message to plugin: "<< (*iter)->name() << std::endl;
	//	OmniPlugin::passMessage((*iter), msg);
	}

	return list.str();
}

OmniCommChannel* PluginManager::setupChannel(std::string name)
{
	OmniPlugin* target = NULL;
	for(int i = 0; i < _plugins.size(); ++i)
	{
		if(!_plugins[i]->name().compare(name))
		{
			target = _plugins[i];
			break;
		}
			
	}

	if(target == NULL)
	{
		return NULL;
	}

	OmniCommEndpoints eps = _commChannels->getNewEndpoints();

	OmniPlugin::passOmniCommChannel(target, eps.second);
	
	return eps.first;
	
}
