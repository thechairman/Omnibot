#include <sstream>
#include "OmniDropCmd.h"

const std::string OmniDropCmd::NAME = "drop";
const std::string OmniDropCmd::USE = "drop [ plugin ... ]";
const std::string OmniDropCmd::HELP = "Unloads the specified loaded plugins";

/*void OmniDropCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniDropCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}*/
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
/*std::string OmniDropCmd::resultStr()
{
	return _results;
}
void OmniDropCmd::clear()
{
	_results = "";
}*/
bool OmniDropCmd::exec(ircMessage& msg)
{
	std::istringstream tokenizer(msg.message());
	std::string token;
	bool foundCommand = false;

	while(getline(tokenizer, token, ' '))
	{
		if(token.compare(name()) == 0)
		{
			foundCommand = true;
			continue;
		}

		if(_plugins->unload(token))
		{
			_irc->sendMessage(msg.channel(), "dropping " + token);
		}
		else
		{
			_irc->sendMessage(msg.channel(), "sorry " +  msg.user().nick() +", I can't find that plugin");
		}
	}

	return false;
}
/*bool OmniDropCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}*/

