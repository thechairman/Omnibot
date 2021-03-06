#ifndef _IRC_USERS_INTERFACE_
#define _IRC_USERS_INTERFACE_
#include "ircUserDB.h"
#include "ircNotifyClasses.h"

class ircUsersInterface{
	public:
		ircUsersInterface();
		ircUsersInterface(ircUserDB*);
		~ircUsersInterface();

		std::vector<ircUser> getChannelUsers(std::string);
		ircUser getUser(std::string);
		ircUser getUser(unsigned int);

		std::vector<ircUser> getAllUsers();
		std::vector<ircUser> getOnlineRegisteredUsers();
		std::vector<ircUser> getAllRegisteredUsers();
		std::vector<ircUser> getAuthenticatedUsers();
	private:
		ircUserDB* _DB;
		std::vector<ircUser> pointersToCopies(std::vector<ircUser*>);
};

#endif
