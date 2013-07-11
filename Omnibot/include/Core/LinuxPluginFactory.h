#include<PluginFactory.h>

class LinuxPluginFactory:public PluginFactory{
	public:
	LinuxPluginFactory();
	OmniPlugin* load(std::string);
	void unload(std::string);
	std::string listPlugins();
	~LinuxPluginFactory();

	private:
	std::map<std::string, void*> handles;
};
