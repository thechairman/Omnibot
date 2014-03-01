#include <cstdlib>
#include <sstream>
#include <cstdlib>
#include "OmniQuitCmd.h"

const std::string OmniQuitCmd::NAME = "quit";
const std::string OmniQuitCmd::USE = "quit";
const std::string OmniQuitCmd::HELP = "Instructs the bot to quit the server and termintate the program";

/*void OmniQuitCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniQuitCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}*/
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
/*std::string OmniQuitCmd::resultStr()
{
	return _results;
}
void OmniQuitCmd::clear()
{
	_results = "";
}*/
bool OmniQuitCmd::exec(ircMessage& msg)
{

	_irc->quit();
	exit(0);

	return false;
}
/*bool OmniQuitCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}*/
