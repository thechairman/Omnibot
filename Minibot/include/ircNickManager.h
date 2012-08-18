#include "ircInterface.h"
#include "ircUserDB.h"

class ircUserManager {
	
	public:

		
		ircUserManger(ircInterface*);
		~ircUserManager();

		void setAuthMethod(std::string);


		//public pass throughs to the user database
		ircUser* getUser(std::string);
		ircUser* getUser(unsigned int);
		std::vector<ircUser*> getChannelCurrentUsers(std::string);

	private:

		void setNickModePrefixes(std::string);

		//private pass throughs to the user database 
		//primarily used by the ircInterface 
		void addUser(std::string);
		void addUser(std::string, bool, bool);
		void removeUser(std::string);
		void addUserToChannel(std::string, std::string);
		void removeUserFromChannel(std::string, std::string);

		ircUserDB* _userDB;
		ircInterface* _irc;

	friend class ircInterface;
}


