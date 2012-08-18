#include "ircUsersInterface.h"
#include "ircNotifyClasses.h"

ircUsersInterface::ircUsersInterface(): _DB(NULL){}
ircUsersInterface::ircUsersInterface(ircUserDB* db): _DB(db){}
ircUsersInterface::~ircUsersInterface(){}

ircUser ircUsersInterface::getUser(std::string nick)
{
	return *(_DB->getUser(nick));
}
ircUser ircUsersInterface::getUser(unsigned int id)
{
	return *(_DB->getUser(id));
}
std::vector<ircUser> ircUsersInterface::getChannelUsers(std::string channel)
{
	std::vector<ircUser*> users = _DB->getChannelCurrentUsers(channel);
	return pointersToCopies(users);
}
std::vector<ircUser> ircUsersInterface::getAllUsers()
{
	std::vector<ircUser*> users = _DB->allUsers();
	return pointersToCopies(users);
}
std::vector<ircUser> ircUsersInterface::getOnlineRegisteredUsers()
{
	std::vector<ircUser*> users = _DB->onlineRegisteredUsers();
	return pointersToCopies(users);
}
std::vector<ircUser> ircUsersInterface::getAllRegisteredUsers()
{
	std::vector<ircUser*> users = _DB->allRegisteredUsers();
	return pointersToCopies(users);
}
std::vector<ircUser> ircUsersInterface::getAuthenticatedUsers()
{
	std::vector<ircUser*> users = _DB->authenticatedUsers();
	return pointersToCopies(users);
}

std::vector<ircUser> ircUsersInterface::pointersToCopies(std::vector<ircUser*> users)
{
	std::vector<ircUser> retval;
	std::vector<ircUser*>::iterator iter = users.begin();
	for(iter = users.begin(); iter != users.end(); iter++)
	{
		retval.push_back(*(*iter));
	}

	return retval;
}
