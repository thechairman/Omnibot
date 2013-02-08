#include <sstream>
#include "OmniListCmd.h"

const std::string OmniListCmd::NAME = "list";
const std::string OmniListCmd::USE = "list";
const std::string OmniListCmd::HELP = "Lists all loaded plugins";

/*void OmniListCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniListCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}*/
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
/*std::string OmniListCmd::resultStr()
{
	return _results; 
}
void OmniListCmd::clear()
{
	_results = "";
}*/
bool OmniListCmd::exec(ircMessage& msg){
	_irc->sendMessage(msg.channel(), "Loaded Plugins: " +_plugins->listLoadedPlugins());
	return false;
}
/*bool OmniListCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}*/
