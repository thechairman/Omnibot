#include<PluginFactory.h>

class StaticPluginFactory:public PluginFactory{
	public:
	StaticPluginFactory();
	OmniPlugin* load(std::string);
	void unload(std::string);
	std::string listPlugins();
	~StaticPluginFactory();
};
