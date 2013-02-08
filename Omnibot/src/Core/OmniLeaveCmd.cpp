#include <sstream>
#include "OmniLeaveCmd.h"

const std::string OmniLeaveCmd::NAME = "leave";
const std::string OmniLeaveCmd::USE = "leave <channel name>";
const std::string OmniLeaveCmd::HELP = "Instructs the bot to leave the channel";

/*void OmniLeaveCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniLeaveCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}*/
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
/*std::string OmniLeaveCmd::resultStr()
{
	return _results;
}
void OmniLeaveCmd::clear()
{
	_results = "";
}*/
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
/*bool OmniLeaveCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}*/
