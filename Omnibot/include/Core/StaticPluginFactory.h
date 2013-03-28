#include<PluginFactory.h>

class StaticPluginFactory:public PluginFactory{
	public:
	StaticPluginFactory();
	OmniPlugin* load(std::string);
	~StaticPluginFactory();
};
