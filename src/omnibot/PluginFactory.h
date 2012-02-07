#ifndef _PLUGIN_FACTORY_
#define _PLUGIN_FACTORY_

#include "OmniPluginInterface.h"

/* this class is designed to wrap system specific
 * dynamic class loading up into a factory class
 * so you it enhances code portabilty, it 
 * should probably be an interface inherited by 
 * specific implementations
 */
class PluginFactory;
class OmniPlugin;


class PluginFactory{

	public:
	static PluginFactory* instance();
	virtual ~PluginFactory();
	virtual OmniPlugin* load(std::string) = 0;

	protected:
	PluginFactory();

	private:
	static PluginFactory* _instance;

};

#endif

