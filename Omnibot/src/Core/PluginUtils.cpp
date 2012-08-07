#include "PluginUtils.h"

PluginUtils::PluginUtils(PluginUtils* utils, PluginAttrs* attrs)
{
	_irc = utils->_irc;
	_manager = utils->_manager;
	_nicks = utils->_nicks;
	_attrs = attrs;
}
PluginUtils::PluginUtils(ircInterface* irc_, PluginManager& manager_, NickManager& nicks_): _irc(irc_),_manager(&manager_),_nicks(&nicks_){}


void PluginUtils::sendPM(std::string nick, std::string message){
	_irc->sendPM(nick, message);
}


void PluginUtils::sendMessage(std::string channel, std::string message){
	_irc->sendMessage(channel, message);
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
	return _manager->setupChannel(name);
}
