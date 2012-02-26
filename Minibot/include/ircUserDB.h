#include "ircNotifyClasses.h"

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <climits>
class ircUserDB {

	public:	
		ircUserDB();
		~ircUserDB();
		void addUser(std::string, bool, bool);
		void removeUser(std::string);
		void addUserToChannel(std::string, std::string);
		void removeUserFromChannel(std::string, std::string);
		ircUser* getUser(std::string);
		ircUser* getUser(unsigned int);
		std::vector<ircUser*> getChannelCurrentUsers(std::string);

	private:
		static const unsigned int RANGE = UINT_MAX;
		std::map<std::string, unsigned int> _registeredNicksToIDs;
		std::map<std::string, ircUser*> _usersByNick;
		std::map<unsigned int, ircUser*> _usersByID;
		std::map<std::string, std::vector<ircUser*> > _usersByChannel;

		typedef usersByNick_it std::map<std::string, ircUser*>::iterator;
		typedef usersByID_it std::map<unsigned int, ircUser*>::iterator;
	      	typedef registeredId_it std::map<std::string, unsigned int>::iterator;
		typedef channels_it std::map<std::string, std::vector<ircUser*> >::iterator;
		typedef nicklist_it std::std::vector<ircUser*>::itrator;
	
		unsigned int getNextAvailableID();
		void saveData();
		void loadData();

}
