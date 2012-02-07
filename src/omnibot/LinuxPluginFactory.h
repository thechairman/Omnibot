#include<PluginFactory.h>

class LinuxPluginFactory:public PluginFactory{
	public:
	LinuxPluginFactory();
	OmniPlugin* load(std::string);
	~LinuxPluginFactory();
};
