#ifndef _IRC_POSIX_USERDB_
#define _IRC_POSIX_USERDB_
#include "ircNotifyClasses.h"
#include "ircUserDB.h"
#include "ircTypes.h"

#include <pthread.h>

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <climits>

class posix_ircUserDB: public ircUserDB {

	public:	

		static const ircUserId_t INVALID_ID = 0;
		
		posix_ircUserDB();
		~posix_ircUserDB();
		void addUser(std::string, bool, bool);
		void removeUser(std::string);
		void addUserToChannel(std::string, std::string);
		void removeUserFromChannel(std::string, std::string);
		void nickChange(std::string, std::string);
		ircUser* getUser(std::string);
		ircUser* getUser(ircUserId_t);
		std::vector<ircUser*> getChannelCurrentUsers(std::string);

		std::vector<ircUser*> allUsers();
		std::vector<ircUser*> onlineRegisteredUsers();
		std::vector<ircUser*> allRegisteredUsers();
		std::vector<ircUser*> authenticatedUsers();

		void lockUsers();
		void releaseUsers();

		void printAllUsers();
		void printChannelUsers(std::string);

	private:
		std::ofstream dbLog;

		//methods for ircINterface use
		void setRegistered(std::string,bool);
		void setAuthenticated(std::string, bool);
		void setRegistered(ircUserId_t, bool);
		void setAuthenticated(ircUserId_t, bool);

		//mutexes
		pthread_mutex_t theBigLock;
		pthread_mutex_t userLock;
		void lockDB();
		void unlockDB();

		static const unsigned int RANGE = UINT_MAX;
		ircRegisteredUserIndex _regsiteredUsers
		ircNickIndex _nickIndex;
		ircServer _server;

	
		ircUserId_t getNextAvailableID();
		void saveData();
		void loadData();

		friend class ircUserAuth;

};

#endif
