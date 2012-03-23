#ifndef _IRC_POSIX_USERDB_
#define _IRC_POSIX_USERDB_
#include "ircNotifyClasses.h"
#include "ircUserDB.h"

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

		posix_ircUserDB();
		~posix_ircUserDB();
		void addUser(std::string, bool, bool);
		void removeUser(std::string);
		void addUserToChannel(std::string, std::string);
		void removeUserFromChannel(std::string, std::string);
		void nickChange(std::string, std::string);
		ircUser* getUser(std::string);
		ircUser* getUser(unsigned int);
		std::vector<ircUser*> getChannelCurrentUsers(std::string);

		std::vector<ircUser*> allUsers();
		std::vector<ircUser*> onlineRegisteredUsers();
		std::vector<ircUser*> allRegisteredUsers();
		std::vector<ircUser*> authenticatedUsers();

		void printAllUsers();
		void printChannelUsers(std::string);

	private:
		//methods for ircINterface use
		void setRegistered(std::string,bool);
		void setAuthenticated(std::string, bool);
		void setRegistered(unsigned int, bool);
		void setAuthenticated(unsigned int, bool);

		//mutexes
		pthread_mutex_t theBigLock;
		pthread_mutex_t userLock;

		static const unsigned int RANGE = UINT_MAX;
		std::map<std::string, unsigned int> _registeredNicksToIDs;
		std::map<std::string, ircUser*> _usersByNick;
		std::map<unsigned int, ircUser*> _usersByID;
		std::map<std::string, std::vector<ircUser*> > _usersByChannel;

		typedef std::map<std::string, ircUser*>::iterator usersByNick_it;
		typedef std::map<unsigned int, ircUser*>::iterator usersByID_it;
	      	typedef std::map<std::string, unsigned int>::iterator registeredId_it;
		typedef std::map<std::string, std::vector<ircUser*> >::iterator channels_it;
		typedef std::vector<ircUser*>::iterator nicklist_it;
	
		unsigned int getNextAvailableID();
		void saveData();
		void loadData();

		friend class ircUserAuth;

};

#endif
