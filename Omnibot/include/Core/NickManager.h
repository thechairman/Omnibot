#ifndef _NICK_MANAGER_
#define _NICK_MANAGER_

//#include"ircUsersInterface.h"
#include"ircNotifyClasses.h"
#include"ircInterface.h"

#include<string>
#include<vector>

class NickManager{

	public:
		NickManager();
		~NickManager();

		std::vector<ircUser> getChannelUsers(std::string);
		ircUser getUser(std::string);
		ircUser getUser(unsigned int);

		std::vector<ircUser> getAllUsers();
		std::vector<ircUser> getOnlineRegisteredUsers();
		std::vector<ircUser> getAllRegisteredUsers();
		std::vector<ircUser> getAuthenticatedUsers();

	private:
//		ircUsersInterface* _UI;
};
	
#endif
