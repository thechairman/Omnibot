#include "ircUserDB.h"

ircUserDB::ircUserDB()
{
	srand(time(NULL));
	loadData();

}

ircUserDB::~ircUserDB()
{
	saveData();
}

void ircUserDB::addUser(std::string nick bool registered, bool authenticated)
{
	usersByNick_it iter = _usersByNick.find(nick);
	if(iter == std::map::end){
		//create new user object
		ircUser* temp = new ircUser();
		temp->nick= nick;

		registerId_it idIter = _registeredNicksToIDs.find(nick);
		if(registered || idIter != std::map::end)
		{
			temp->_userId = (*idIter);
		}
		else
		{
			//generate a new id
			temp->userId() = getNextAvailableID();
		}
i			temp->_isAuthenticated = authenticated;

		
		//add it to both maps
		_usersByNick[nick] = temp;
		_usersById[temp->userId()] = temp;
	}
	//else its already here so whe shouldn't need to add it
}

//TODO this should be redone with channels by nick
void ircUserDB::removeUser(std::string nick)
{
	//get user object
	usersByNick_it user_i = _usersByNick.find(nick);
	if(user_i == std::map::end)
	{
		//nick doen't exist
		return;
	}

	ircUser* user = (*user_i)
	
	//remove it forom channels;
	channels_it channels;
	for(channels = _usersByChannel.begin(); channels != _usersByChannel.end(); channels++)
	{
		nicklist_it nicks;
		for (nicks = (*channels).begin(); nicks != (*channels).end(); nicks++){
			if((*nicks)->userId() == user->userId())
			{
				(*channels).erase(nicks);
				break;
			}
		}
	}
	
	//remove from usersbyid
	_usersByID.erase(_usersByID.find(user->userId()));
	
	//remove from users by nick
	_usersByNick.erase(_usersByNick.find(user->nick()));

	delete user;
}	

void ircUserDB::addUserToChannel(std::string nick, std::string channel)
{
	usersByNick_it user = _usersByNick.find(nick);

	_usersByChannel[channel].push_back((*user));
}

void ircUserDB::removeUserFromChannel(std::string nick, std::string channel)
{
	std::vector<ircUser*> users =  _usersByChannel[channel];

	unsigned int userId = (*(_usersByNick.find(nick)))->userId();

	std::vector<ircUser*>::iterator channelUser;
	for(channelUser = users.begin(); channelUser != users.end(); channelUser++)
	{
		if((*channelUser)->userId() == userId)
		{
			users.erase(channelUser);
		}
	}
}

ircUser* ircUserDB::getUser(std::string nick)
{
	return _usersByNick[nick];
}

ircUser* ircUserDB::getUser(unsigned int ID)
{
	return _usersByNick[ID];
}

std::vector<ircUser*> ircUserDB::getChannelCurrentUsers(std::string channel)
{
	return _usersByChannel[channel];
}

unsigned int ircUserDB::getNextAvailableID()
{

	unsigned int posId;

	/* This actually works correctly. */
#define RAND_INV_RANGE(r) ((unsigned int) ((RAND_MAX + 1) / (r)))

	do {
	    posId = (unsigned int) rand();
	} while (posId >= RANGE * RAND_INV_RANGE (RANGE));
	posId /= RAND_INV_RANGE (RANGE);

#undef RAND_INV_RANGE

	registeredId_it regs = _regsisteredNickstoIDs.find(posId);
	usersByID_it active = _usersByID.find(posId);

	if(regs != std::map::end || regs != std::map::end)
	{
		return getNextAvailableID();
	}
	else
	{
		return posId;
	}
}

void ircUserDB::saveData()
{

	std::ofstream dbfile;
	dbfile.open("users.db");
	
	dbfile.write(_registeredNicksToIDs.size())
	for(registeredId_it iter = _registeredNicksToIDs.begin(); iter != _registeredNicksToIDs.end(); iter++)
	{
		dbfile.write(iter->first);
		dbfile.write(iter->second);
	}
}

void ircUserDB::loadData()
{
	std::ifstream dbfile;
	dbfile.open("users.db");

	size_t mapsize = dbfile.read();

	for int(size_t i = 0; size_t < mapsize; ++i)
	{
		std::string nick = dbfile.read();
		unsigned int id  = dbfile.read();
		_registeredNicksToIDs[nick] = id;

	}
}
