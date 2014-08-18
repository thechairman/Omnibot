#include "PluginUtils.h"

#include <iostream>
PluginUtils::PluginUtils(PluginUtils* utils, PluginAttrs* attrs)
{
	_irc = utils->_irc;
	_manager = utils->_manager;
	_nicks = utils->_nicks;
	_attrs = attrs;
}
PluginUtils::PluginUtils(ircInterface* irc_, PluginManager& manager_, NickManager& nicks_): _irc(irc_),_manager(&manager_),_nicks(&nicks_){}


void PluginUtils::sendPM(std::string nick, std::string message){
	if(_attrs->canPrivateMessage())
	{
		_irc->sendPM(nick, message);
	}
}


void PluginUtils::sendMessage(std::string channel, std::string message)
{
	if(_attrs->canChannelMessage())
	{
		_irc->sendMessage(channel, message);
	}
}
ircUser PluginUtils::getUser(std::string nick)
{
	return _nicks->getUser(nick);
}
ircUser PluginUtils::getUser(unsigned int id)
{
	return _nicks->getUser(id);
}

std::vector<ircUser> PluginUtils::getChannelUsers(std::string channel)
{
	return _nicks->getChannelUsers(channel);
}
std::vector<ircUser> PluginUtils::getAllUsers()
{
	return _nicks->getAllUsers();
}
std::vector<ircUser> PluginUtils::getOnlineRegisteredUsers()
{
	return _nicks->getOnlineRegisteredUsers();
}
std::vector<ircUser> PluginUtils::getAllRegisteredUsers()
{
	return _nicks->getAllRegisteredUsers();
}
std::vector<ircUser> PluginUtils::getAuthenticatedUsers()
{
}

OmniCommChannel* PluginUtils::openOmniCommChannel(std::string name)
{
	OmniCommChannel* retval = NULL; 
	if(_attrs->canTalkToPlugins())
	{
		retval = _manager->setupChannel(name);
	}

	 return retval;
}

void PluginUtils::kick(std::string nick)
{
	if(_attrs->canKick())
	{
		std::cout << " I'd kick if I could" << std::endl;
	}

}
void PluginUtils::ban(std::string nick)
{
	if(_attrs->canBan())
	{
		std::cout << " I'd ban if I could" << std::endl;
	}

}
