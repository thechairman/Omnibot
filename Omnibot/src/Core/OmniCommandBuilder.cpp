
#include "OmniCommandBuilder.h"

#include "OmniQuitCmd.h"
#include "OmniLeaveCmd.h"
#include "OmniJoinCmd.h"
#include "OmniLoadCmd.h"
#include "OmniDropCmd.h"
#include "OmniListCmd.h"

#include "ircLog.h"

const std::string FILENAME = "OmniCommandBuilder.cpp";

OmniCommandBuilder::OmniCommandBuilder(){}
OmniCommandBuilder::OmniCommandBuilder(ircInterface* interface, PluginManager* manager, char prefix):
	_prefix(prefix),
	_ircInterface(interface),
	_pluginManager(manager)
{}

void OmniCommandBuilder::getCommands(std::vector<OmniCommand*>& commands)
{

	ircLog::instance()->logf(FILENAME, "the address of commands = %X", &commands);	

	//OmniLoadCmd load;
	commands.push_back(new OmniLoadCmd());

	//OmniDropCmd drop;
	commands.push_back(new OmniDropCmd());

	//OmniJoinCmd join;
	commands.push_back(new OmniJoinCmd());

	//OmniLeaveCmd leave;
	commands.push_back(new OmniLeaveCmd());

	//OmniQuitCmd quit;
	commands.push_back(new OmniQuitCmd());

	//OmniListCmd list;
	commands.push_back(new OmniListCmd());

	setCommandAttributes(commands);
	
	
	for(size_t i = 0; i < commands.size(); ++i)
	{
		ircLog::instance()->logf(FILENAME, "command name: %s", commands[i]->name().c_str());
	}

	ircLog::instance()->logf(FILENAME, "the size of command at the end of getCommands %u",
			commands.size());
}

void OmniCommandBuilder::setPrefix(char prefix)
{
	_prefix = prefix;
}

void OmniCommandBuilder::setTools(ircInterface* irc, PluginManager* plugins)
{
	_ircInterface = irc;
	_pluginManager = plugins;
}

void OmniCommandBuilder::setCommandAttributes(std::vector<OmniCommand*>& commands)
{

	for(size_t i = 0; i < commands.size(); ++i)
	{
		commands[i]->setTools(_ircInterface, _pluginManager);
		commands[i]->setPrefix(_prefix);
	}
}

