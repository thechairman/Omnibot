#include "NickManager.h"

NickManager::NickManager(){}
NickManager::~NickManager(){}

std::vector<ircUser> NickManager::getChannelUsers(std::string channel)
{
	std::vector<ircUser> vct;
	return vct;
	//return _UI->getChannelUsers(channel);
}
ircUser NickManager::getUser(std::string nick)
{
	ircUser user;
	return user;
	//return _UI->getUser(nick);
}
ircUser NickManager::getUser(unsigned int id)
{	
	ircUser user;
	return user;
	//return _UI->getUser(id);
}

std::vector<ircUser> NickManager::getAllUsers()
{
	std::vector<ircUser> vct;
	return vct;
	//return _UI->getAllUsers();
}
std::vector<ircUser> NickManager::getOnlineRegisteredUsers()
{
	std::vector<ircUser> vct;
	return vct;
	//return _UI->getOnlineRegisteredUsers();
}
std::vector<ircUser> NickManager::getAllRegisteredUsers()
{
	std::vector<ircUser> vct;
	return vct;
	//return _UI->getAllRegisteredUsers();
}
std::vector<ircUser> NickManager::getAuthenticatedUsers()
{
	std::vector<ircUser> vct;
	return vct;
	//return _UI->getAllRegisteredUsers();
}

