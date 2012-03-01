#include "ircUserDB.h"

#include <iostream>

ircUserDB::ircUserDB()
{
	srand(time(NULL));
	loadData();

}

ircUserDB::~ircUserDB()
{
	saveData();
}

void ircUserDB::setRegistered(std::string nick,bool registered)
{
	ircUser* user = getUser(nick);
	registeredId_it idIter = _registeredNicksToIDs.find(nick);
	if(registered && idIter != _registeredNicksToIDs.end())
	{
		user->_userId = (*idIter).second;
	}
	else if(registered)
	{
		//temp->_userId = getNextAvailableID();
		_registeredNicksToIDs[nick] = user->_userId;
		saveData();
	}
	
	//else it should have defaulted to false, but lets make sure
	
	
}
void ircUserDB::setAuthenticated(std::string nick, bool authenticated)
{
	ircUser* user = getUser(nick);
	user->_isAuthenticated = authenticated;
}

//*************************************************************
//	not sure this function makes a whole lot of sense
//*************************************************************
void ircUserDB::setRegistered(unsigned int id, bool registered)
{
	ircUser* user = getUser(id);
	registeredId_it idIter = _registeredNicksToIDs.find(user->nick());
	if(registered && idIter != _registeredNicksToIDs.end())
	{
		user->_userId = (*idIter).second;
	}
	else if(registered)
	{
		//temp->_userId = getNextAvailableID();
		_registeredNicksToIDs[user->nick()] = user->_userId;
		saveData();
	}
}
void ircUserDB::setAuthenticated(unsigned int id, bool authenticated)
{
	ircUser* user = getUser(id);
	user->_isAuthenticated = authenticated;
}
void ircUserDB::addUser(std::string nick, bool registered = false, bool authenticated = false)
{
	usersByNick_it iter = _usersByNick.find(nick);
	if(iter == _usersByNick.end())
	{
		//create new user object
		ircUser* temp = new ircUser();
		temp->_nick= nick;

		registeredId_it idIter = _registeredNicksToIDs.find(nick);
		if(registered || idIter != _registeredNicksToIDs.end())
		{
			temp->_userId = (*idIter).second;
		}
		else
		{
			//generate a new id
			temp->_userId = getNextAvailableID();
		}
			temp->_isAuthenticated = authenticated;

		
		//add it to both maps
		_usersByNick[nick] = temp;
		_usersByID[temp->userId()] = temp;
	}
	//else its already here so whe shouldn't need to add it
}

//TODO this should be redone with channels by nick
void ircUserDB::removeUser(std::string nick)
{
	//get user object
	usersByNick_it user_i = _usersByNick.find(nick);
	if(user_i == _usersByNick.end())
	{
		//nick doen't exist
		return;
	}

	ircUser* user = (*user_i).second;

	if(user == NULL)
	{
		std::cout << "this user is broken" << std::endl;
	}
	else
		std::cout << "retrieved user was fine" << std::endl;

	//remove it forom channels;
	channels_it channels;
	for(channels = _usersByChannel.begin(); channels != _usersByChannel.end(); channels++)
	{
		nicklist_it nicks;
		for (nicks = (*channels).second.begin(); nicks != (*channels).second.end(); nicks++){
			if((*nicks)->userId() == user->userId())
			{
				(*channels).second.erase(nicks);
				break;
			}
		}
	}
	
	//remove from usersbyid
	
	std::cout << "just removing it from the arrays now" << std::endl;

	_usersByID.erase(_usersByID.find(user->userId()));
	
	//remove from users by nick
	_usersByNick.erase(_usersByNick.find(user->nick()));

	delete user;
}	

void ircUserDB::addUserToChannel(std::string nick, std::string channel)
{
	usersByNick_it user = _usersByNick.find(nick);

	_usersByChannel[channel].push_back((*user).second);
}

void ircUserDB::removeUserFromChannel(std::string nick, std::string channel)
{
	std::vector<ircUser*> users =  _usersByChannel[channel];

	unsigned int userId = (*(_usersByNick.find(nick))).second->userId();

	std::vector<ircUser*>::iterator channelUser;
	for(channelUser = users.begin(); channelUser != users.end(); channelUser++)
	{
		if((*channelUser)->userId() == userId)
		{
			users.erase(channelUser);
		}
	}

	//see if the nick is any other channels
	channels_it channels;
	for(channels = _usersByChannel.begin(); channels != _usersByChannel.end(); channels++)
	{
		nicklist_it nicks;
		for (nicks = (*channels).second.begin(); nicks != (*channels).second.end(); nicks++){
			if((*nicks)->userId() == userId)
			{
				return;
			}
		}
	}

	//if its not then the user is gon and we need to clean up

	//remove from usersbyid
	_usersByID.erase(_usersByID.find(userId));
	
	//remove from users by nick
	_usersByNick.erase(nick);


}

ircUser* ircUserDB::getUser(std::string nick)
{
	return _usersByNick[nick];
}

ircUser* ircUserDB::getUser(unsigned int ID)
{
	return _usersByID[ID];
}

std::vector<ircUser*> ircUserDB::getChannelCurrentUsers(std::string channel)
{
	return _usersByChannel[channel];
}

unsigned int ircUserDB::getNextAvailableID()
{

	unsigned int posId;

	/* This actually works correctly. */

	//XXX when this was a MACRO it was came back 0 >.>
//#define RAND_INV_RANGE(r) ((unsigned int) ((RAND_MAX + 1) / (r)))
	unsigned int rand_inv_range = (unsigned int) ((RAND_MAX + 1) / RANGE);
	do {
	    posId = (unsigned int) rand();
	} while (posId >= RANGE * rand_inv_range);
	posId /= rand_inv_range;

//#undef RAND_INV_RANGE


	bool exists = false;
	registeredId_it regs;
	for(regs = _registeredNicksToIDs.begin(); regs != _registeredNicksToIDs.end(); regs++)
	{
		if((*regs).second == posId)
		{
			exists = true;
			break;
		}
	}
	usersByID_it active = _usersByID.find(posId);

	if( exists || active != _usersByID.end())
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
	
	dbfile << _registeredNicksToIDs.size();
	for(registeredId_it iter = _registeredNicksToIDs.begin(); iter != _registeredNicksToIDs.end(); iter++)
	{
		dbfile << iter->first;
		dbfile << iter->second;
	}
}

void ircUserDB::loadData()
{
	std::ifstream dbfile;
	dbfile.open("users.db");

	if(!dbfile.good())
		return;

	size_t mapsize;
	dbfile >> mapsize;

	for (size_t i = 0; i < mapsize; ++i)
	{
		std::string nick;
		dbfile >> nick;
		unsigned int id;
		dbfile >> id;
		_registeredNicksToIDs[nick] = id;

	}
}
