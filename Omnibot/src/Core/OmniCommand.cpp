
//command class template for copy pastes
//void OmniCommand::setTools(ircInterface* irc, PluginManager* plugins){}
//void OmniCommand::setPrefix(char newPrefix){}
//std::string OmniCommand::name(){}
//std::string OmniCommand::use(){}
//std::string OmniCommand::help(){}
//std::string OmniCommand::resultStr(){}
//void OmniCommand::clear(){}
//bool OmniCommand::exec(ircMessage& msg){}
//bool OmniCommand::isCommandString(ircMessage& msg){}
//
#include "OmniCommand.h"
#include "ircLog.h"

const std::string FILENAME = "OmniCommand.h";

void OmniCommand::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniCommand::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}
std::string OmniCommand::resultStr()
{
	return _results;
}
void OmniCommand::clear()
{
	_results = "";
}
bool OmniCommand::isCommandString(ircMessage& msg)
{
	ircLog::instance()->logf(FILENAME, "Omni%SCmd: name: %s \t compare: %s",
		       	name().substr(1, name().size() -1).c_str(),
			name().c_str(),
		     	msg.message().substr(0, name().size()).c_str());
	
	int compareVal = msg.message().substr(0, name().size()).compare(name());

	ircLog::instance()->logf(FILENAME, "Omni%SCmd: compareVal = %d",
		       	name().substr(1, name().size() -1).c_str(),
			compareVal);

	return  (compareVal == 0);
}
#if 0
const std::string OmniLoadCmd::NAME = "load";
const std::string OmniLoadCmd::USE ="load [ plugin name ... ]";
const std::string OmniLoadCmd::HELP = "loads a the specified plugins";


void OmniLoadCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniLoadCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}
std::string OmniLoadCmd::name()
{
	return std::string(_prefix) + NAME;
}
std::string OmniLoadCmd::use()
{
	return USE;
}
std::string OmniLoadCmd::help()
{
	return HELP;
}
std::string OmniLoadCmd::resultStr()
{
	return _results;
}
void OmniLoadCmd::clear()
{
	_results = "";
}
bool OmniLoadCmd::exec(ircMessage& msg)
{
	std::istringstream tokenizer(msg.message());
	std::string token;

	while(getline(tokenizer, token, ' '))
	{
		if(token.compare(name()) == 0)
		{
			continue;
		}

		if(_plugins->loadPlugin(token))
		{
			_irc.sendMessage(msg.channel(), token + " loaded...");
		}
		else
		{
			_irc.sendMessage(msg.channel(), token + " failed to load");
		}
	}

	return false;

}
bool OmniLoadCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}

const std::string OmniDropCmd::NAME = "drop";
const std::string OmniDropCmd::USE = "drop [ plugin ... ]";
const std::string OmniDropCmd::HELP = "Unloads the specified loaded plugins";

void OmniDropCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniDropCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}
std::string OmniDropCmd::name()
{
	return std::string(_prefix) + NAME;
}
std::string OmniDropCmd::use()
{
	return USE;
}
std::string OmniDropCmd::help()
{
	return HELP;
}
std::string OmniDropCmd::resultStr()
{
	return _results;
}
void OmniDropCmd::clear()
{
	_results = "";
}
bool OmniDropCmd::exec(ircMessage& msg)
{
	std::istringstream tokenizer(msg.message());
	std::string token;

	while(getline(tokenizer, token, ' '))
	{
		if(token.compare(name()) == 0)
		{
			continue;
		}

		_plugins->dropPlugin(token);
		_irc.sendMessage(msg.channel(), "dropping " + token);
	}

	return false;
}
bool OmniDropCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}

const std::string OmniJoinCmd::NAME = "join";
const std::string OmniJoinCmd::USE = "join <channel name>";
const std::string OmniJoinCmd::HELP = "Instructs the bot to join the specified channel";

void OmniJoinCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniJoinCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;	
}
std::string OmniJoinCmd::name()
{
	return std::string(_prefix) + NAME;
}
std::string OmniJoinCmd::use()
{
	return USE;
}
std::string OmniJoinCmd::help()
{
	return HELP;
}
std::string OmniJoinCmd::resultStr()
{
	return _results;
}
void OmniJoinCmd::clear()
{
	_results = "";
}
std::string OmniJoinCmd::resultStr()
{
	return _results;
}
void OmniJoinCmd::clear(){}
bool OmniJoinCmd::exec(ircMessage& msg)
{
	std::istringstream tokenizer(msg.message());
	std::string token;

	while(getline(tokenizer, token, ' '))
	{
		if(token.compare(name()) == 0)
		{
			continue;
		}
		else
		{
			_irc->join(token);
			break;
		}
	}

	return false;
}
bool OmniJoinCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}

const std::string OmniLeaveCmd::NAME = "leave";
const std::string OmniLeaveCmd::USE = "leave <channel name>";
const std::string OmniLeaveCmd::HELP = "Instructs the bot to leave the channel";

void OmniLeaveCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniLeaveCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}
std::string OmniLeaveCmd::name()
{
	return std::string(_prefix) + NAME;
}
std::string OmniLeaveCmd::use()
{
	return USE;
}
std::string OmniLeaveCmd::help()
{
	return HELP;
}
std::string OmniLeaveCmd::resultStr()
{
	return _results;
}
void OmniLeaveCmd::clear()
{
	_results = "";
}
std::string OmniLeaveCmd::resultStr(){}
void OmniLeaveCmd::clear(){}
bool OmniLeaveCmd::exec(ircMessage& msg)
{

	std::istringstream tokenizer(msg.message());
	std::string token;

	while(getline(tokenizer, token, ' '))
	{
		if(token.compare(name()) == 0)
		{
			continue;
		}
		else
		{
			_irc->part(token);
			break;
		}
	}

	return false;

}
bool OmniLeaveCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}

const std::string OmniQuitCmd::NAME = "quit";
const std::string OmniQuitCmd::USE = "quit";
const std::string OmniQuitCmd::HELP = "Instructs the bot to quit the server and termintate the program";

void OmniQuitCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniQuitCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}
std::string OmniQuitCmd::name()
{
	return std::string(_prefix) + NAME;
}
std::string OmniQuitCmd::use()
{
	return USE;
}
std::string OmniQuitCmd::help()
{
	return HELP;
}
std::string OmniQuitCmd::resultStr()
{
	return _results;
}
void OmniQuitCmd::clear()
{
	_results = "";
}
std::string OmniQuitCmd::resultStr(){}
void OmniQuitCmd::clear(){}
bool OmniQuitCmd::exec(ircMessage& msg)
{

	_irc->quit();
	exit(0);

	return false;
}
bool OmniQuitCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}

const std::string OmniListCmd::NAME = "list";
const std::string OmniListCmd::USE = "list";
const std::string OmniListCmd::HELP = "Lists all loaded plugins";

void OmniListCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniListCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}
std::string OmniListCmd::name()
{
	return std::string(_prefix) + NAME;
}
std::string OmniListCmd::use()
{
	return USE;
}
std::string OmniListCmd::help()
{
	return HELP;
}
std::string OmniListCmd::resultStr()
{
	return 
}
void OmniListCmd::clear()
{
	_results = "";
}
bool OmniListCmd::exec(ircMessage& msg){
	_irc->sendMessage(msg.channel(), "Loaded Plugins: " +_plugins->listLoadedPlugins());
}
bool OmniListCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}

#include "OmniCommandBuilder.h"

#include "OmniQuitCmd.h"
#include "OmniLeaveCmd.h"
#include "OmniJoinCmd.h"
#include "OmniLoadCmd.h"
#include "OmniDropCmd.h"
#include "OmniListCmd.h"

OmniCommandBuilder::OmniCommandBuilder(){}
OmniCommandBuilder::OmniCommandBuilder(ircInterface* interface, pluginManager* manager, char prefix):
	_ircInterface(interface),
	_pluginManager(manager),
	_prefix(prefix)
{}

void OmniCommandBuilder::getCommands(std::vector<OmniCommand>& commands)
{
	OmniLoadCmd load;
	commands.push_back(load);

	OmniDropCmd drop;
	commands.push_back(drop);

	OmniJoinCmd join;
	commands.push_back(join);

	OmniLeaveCmd leave;
	commands.push_back(leave);

	OmniQuitCmd quit;
	commands.push_back(quit);

	OmniListCmd list;
	commands.push_back(list);

	setCommandAttributes(commands);
}

void OmniCommandBuilder::setPrefix(char prefix)
{
	_prefix = prefix;
}

void OmniCommandBuilder::setTools(ircInterface* irc, pluginManager* plugins)
{
	_ircInterface = irc;
	_pluginManager = plugins;
}

void OmniCommandBuilder::setCommandAttributes(std::vector<OmniCommand>& commands)
{

	for(size_t i = 0; i < commands.size(); ++i)
	{
		commands[i].setTools(_ircInterface, _pluginManager);
		commands[i].setPrefix(_prefix);
	}
}
#endif
