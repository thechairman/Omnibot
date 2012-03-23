#include "NickManager.h"

NickManager::NickManager(ircUsersInterface* ui):_UI(ui){}
NickManager::~NickManager(){}

std::vector<ircUser> NickManager::getChannelUsers(std::string channel)
{
	return _UI->getChannelUsers(channel);
}
ircUser NickManager::getUser(std::string nick)
{
	return _UI->getUser(nick);
}
ircUser NickManager::getUser(unsigned int id)
{
	return _UI->getUser(id);
}

std::vector<ircUser> NickManager::getAllUsers()
{
	return _UI->getAllUsers();
}
std::vector<ircUser> NickManager::getOnlineRegisteredUsers()
{
	return _UI->getOnlineRegisteredUsers();
}
std::vector<ircUser> NickManager::getAllRegisteredUsers()
{
	return _UI->getAllRegisteredUsers();
}
std::vector<ircUser> NickManager::getAuthenticatedUsers()
{
	return _UI->getAllRegisteredUsers();
}

