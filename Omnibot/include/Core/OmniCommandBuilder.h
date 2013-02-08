
#ifndef _OMNI_COMMAND_BUILDER_
#define _OMNI_COMMAND_BUILDER_

#include <vector>

#include "OmniCommand.h"


class OmniCommandBuilder{
	public:
		OmniCommandBuilder();
		OmniCommandBuilder(ircInterface*, PluginManager*, char);
		void getCommands(std::vector<OmniCommand*>&);
		void setPrefix(char);
		void setTools(ircInterface*, PluginManager*);
	private:
		char _prefix;
		ircInterface* _ircInterface;
		PluginManager* _pluginManager;


		void setCommandAttributes(std::vector<OmniCommand*>&);

};


#endif
