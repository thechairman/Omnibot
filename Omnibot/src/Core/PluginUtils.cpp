#include "PluginUtils.h"

PluginUtils::PluginUtils(PluginUtils* utils, PluginAttrs* attrs)
{
	_irc = utils->_irc;
	_manager = utils->_manager;
	_nicks = utils->_nicks;
	_attrs = attrs;
}
PluginUtils::PluginUtils(ircInterface* irc_, PluginManager& manager_, NickManager& nicks_): _irc(irc_),_manager(&manager_),_nicks(nicks_){}


void PluginUtils::sendPM(std::string nick, std::string message){
	_irc->sendPM(nick, message);
}

