#include <map>`
#include <vector>

#include "ircTypes.h"

class ircUserIdClient
{
	public:
	virtual bool idInUse(ircuserid_t);
};

//implemented
class ircUserIdExpert
{
	public:
	static const ircUserId_t INVALID_ID = 0;
	ircUserId_t getNextAvailableID();
	void addClientObject(ircUserIdClient*);
	private:
	std::vector<ircUserIdClient*> _clients;
}

class ircNickIndex: public ircUserIdClient
{
	public: 
	ircUser* getUser(std::string);
	ircUser* getUser(ircUserId_t);
	void addUser(ircUser*);
	ircUser* removeUser(std::string);
	ircUser* removeUser(ircUserId_t);
	ircUser*  removeUser(ircUser*);
	void changeNick(std::string nick, std::string newNick);
	std::vector<ircUser*>* allUsers();
	std::vector<ircUser*>* authenticatedUsers();
	bool userExists(ircUser*);
	bool idInUse(ircuserid_t);
	void printAllUsers();

	typedef std::map<ircUserId_t, ircUser*>::iterator iterator;
	iterator begin();
	iterator end();

	private:
	std::map<std::string, ircUser*> _usersByNick;
	std::map<ircUserId_t, ircUser*> _usersByID;
};

//implemented
class ircRegisteredUserIndex: public ircUserIdClient
{
	public:
	typedef std::map<std::string, ircUserId_t>::iterator iterator;
	iterator begin();
	iterator end();

	ircUserId_t getId(std::string);
	void registerUser(ircUser*);
	bool IdIsRegistered(ircUserId_t);
	bool idInUse(ircUserId_t);
	void saveData();
	void loadData();

	private:
	std::map<std::string, ircUserId_t> _registeredNicksToIds;

};

//implemented
class ircChannel
{
	public:
	ircChannel(std::string);
	void addUser(ircUser*);
	void removeUser(ircUser*);
	bool isUserPresent(ircUser*);
	void printChannelUsers()
	private:
	std::vector<ircUser*>::iterator findUser();
	std::string _name;
	std::vector<ircUser*> _nicklist;
};

//implemented
class ircServer
{
	public:
	ircChannel getChannel(std::string);
	void createChannel(std::string);
	bool channelExists(std::string);
	void removeChannel(std::string);
	void removeUser(ircUser*);
	void addUserToChannel(ircUser*, std::string);
	void removeUserFromChannel(ircUser*, std::string);
	bool userIsPresent(ircUser*);
	void printChannelUsers(std::string);

	private:
	std::map<std::string, ircChannel*> channels;	
};
