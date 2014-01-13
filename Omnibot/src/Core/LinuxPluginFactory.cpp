
#include <dlfcn.h>
#include <dirent.h>

#include <LinuxPluginFactory.h>
#include <OmniConfigParser.h>

//#include"hurlbot.h"
//#include"bashbot.h"
//#include"qdbbot.h"
//#include"tomebot.h"
//#include"8ballbot.h"

OmniPlugin* LinuxPluginFactory::load(std::string pluginName){

	OmniPlugin* plugin = NULL;
//	if(!pluginName.compare("hurlbot")){
//		plugin = dynamic_cast<OmniPlugin*>  (new hURLbot());
//	}
//	else if(!pluginName.compare("bashbot")){
//		plugin = dynamic_cast<OmniPlugin*>  (new bashbot());
//	}
//	else if(!pluginName.compare("qdbbot")){
//		plugin = dynamic_cast<OmniPlugin*>  (new qdbbot());
//	}
///*	else if(!pluginName.compare("tomebot")){
//		plugin = dynamic_cast<OmniPlugin*>  (new tomebot());
//	}*/
//	else if(!pluginName.compare("8ballbot")){
//		plugin = dynamic_cast<OmniPlugin*>  (new eightBallBot());
//	}
//
//	return plugin;
//	

	std::string pluginLib = OmniConfigParser::instance()->pluginDirectory();

	//add the slash if its not already there.
	if(pluginLib[pluginLib.size() -1] != '/')
	{
		pluginLib += "/";
	}

	pluginLib += "lib" + pluginName + ".so";

	void* libHandle = dlopen(pluginLib.c_str(), RTLD_LAZY);

	if(!libHandle)
	{
		return plugin;
	}

	handles[pluginName] = libHandle;

	OmniPlugin* (*CreatePlugin)();
        CreatePlugin = (OmniPlugin* (*)()) dlsym(libHandle, "CreatePlugin");	

	plugin = (*CreatePlugin)();

	return plugin;

}

void LinuxPluginFactory::unload(std::string pluginName)
{

	void* libHandle;

	std::map<std::string, void*>::iterator iter;
	
	iter = handles.find(pluginName);

	if(iter != handles.end())
	{

		libHandle = iter->second;
		dlclose(libHandle);
	}



}


std::string LinuxPluginFactory::listPlugins()
{
	std::vector<std::string> plugins;
	std::string allPlugins = "";

	DIR* dir;
	struct dirent *ent;

	dir = opendir(OmniConfigParser::instance()->pluginDirectory().c_str());
	if(dir != NULL)
	{
		//read directory entries
		ent = readdir(dir);
		while(ent != NULL)
		{

			ent = readdir(dir);
			std::string temp(ent->d_name);
			plugins.push_back(temp);

		}

		closedir(dir);

		//get the plugin files
		for(size_t i = 0; i < plugins.size(); ++i)
		{
			size_t extension = plugins[i].find(".so");
			if(extension != std::string::npos)
			{
				allPlugins += plugins[i].substr(0, extension) + " ";
			}
		}

	}
	else
	{	
		allPlugins = "ERROR";
	}


	return allPlugins;
}

LinuxPluginFactory::~LinuxPluginFactory(){}
LinuxPluginFactory::LinuxPluginFactory(){}
