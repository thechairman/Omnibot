#ifndef _IRC_USERDB_
#define _IRC_USERDB_

#include "ircNotifyClasses.h"

#include <vector>
#include <string>

class ircUserDB {
	public:
		friend class ircInterface;
		friend class ircUserAuth;

		virtual void addUser(std::string, bool, bool) = 0;
		virtual void removeUser(std::string) = 0;
		virtual void addUserToChannel(std::string, std::string) = 0;
		virtual void removeUserFromChannel(std::string, std::string) = 0;
		virtual void nickChange(std::string, std::string) = 0;
		virtual ircUser* getUser(std::string) = 0;
		virtual ircUser* getUser(unsigned int) = 0;
		virtual std::vector<ircUser*> getChannelCurrentUsers(std::string) = 0;

		virtual void printAllUsers() = 0;
		virtual void printChannelUsers(std::string) = 0;

		static ircUserDB* create();

	private:
		virtual void setRegistered(std::string,bool) = 0;
		virtual void setAuthenticated(std::string, bool) = 0;
		virtual void setRegistered(unsigned int, bool) = 0;
		virtual void setAuthenticated(unsigned int, bool) = 0;
};
#endif
