
#include <sstream>

#include "OmniJoinCmd.h"


const std::string OmniJoinCmd::NAME = "join";
const std::string OmniJoinCmd::USE = "join <channel name>";
const std::string OmniJoinCmd::HELP = "Instructs the bot to join the specified channel";

/*void OmniJoinCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniJoinCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;	
}*/
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
/*std::string OmniJoinCmd::resultStr()
{
	return _results;
}
void OmniJoinCmd::clear()
{
	_results = "";
}*/
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
/*bool OmniJoinCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}*/
