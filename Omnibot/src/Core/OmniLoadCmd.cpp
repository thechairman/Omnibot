
#include <sstream>
#include "OmniLoadCmd.h"

const std::string OmniLoadCmd::NAME = "load";
const std::string OmniLoadCmd::USE ="load [ plugin name ... ]";
const std::string OmniLoadCmd::HELP = "loads a the specified plugins";


/*void OmniLoadCmd::setTools(ircInterface* irc, PluginManager* plugins)
{
	_irc = irc;
	_plugins = plugins;
}
void OmniLoadCmd::setPrefix(char newPrefix)
{
	_prefix = newPrefix;
}*/
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
/*std::string OmniLoadCmd::resultStr()
{
	return _results;
}
void OmniLoadCmd::clear()
{
	_results = "";
}*/
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

		if(_plugins->load(token))
		{
			_irc->sendMessage(msg.channel(), token + " loaded...");
		}
		else
		{
			_irc->sendMessage(msg.channel(), token + " failed to load");
		}
	}

	return false;

}
/*bool OmniLoadCmd::isCommandString(ircMessage& msg)
{
	return msg.message().substr(0, name().size()).compare(name()) == 0;
}*/
