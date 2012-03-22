#include "posix_ircUserDB.h"

#include <iostream>
#include <sstream>
posix_ircUserDB::posix_ircUserDB()
{
	std::cout << "posix_ircUserDB: initializing rand..." << std::endl;
	srand(time(NULL));

	std::cout << "posix_ircUserDB: loading data..." << std::endl;

	loadData();
}

posix_ircUserDB::~posix_ircUserDB()
{
	saveData();
}

void posix_ircUserDB::setRegistered(std::string nick,bool registered)
{
	ircUser* user = getUser(nick);
	
	if(user == NULL)
	{
		std::cout << "setRegistered" << std::endl;
		printAllUsers();
	}

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
void posix_ircUserDB::setAuthenticated(std::string nick, bool authenticated)
{
	ircUser* user = getUser(nick);

	if(user == NULL)
	{
		std::cout << "setAauthenticated" << std::endl;
		printAllUsers();
	}
	user->_isAuthenticated = authenticated;
}

//*************************************************************
//	not sure this function makes a whole lot of sense
//*************************************************************
void posix_ircUserDB::setRegistered(unsigned int id, bool registered)
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
void posix_ircUserDB::setAuthenticated(unsigned int id, bool authenticated)
{
	ircUser* user = getUser(id);
	user->_isAuthenticated = authenticated;
}
void posix_ircUserDB::addUser(std::string nick, bool registered = false, bool authenticated = false)
{
	usersByNick_it iter = _usersByNick.find(nick);
	if(iter == _usersByNick.end() || (*iter).second == NULL)
	{
		std::cout << "posix_ircUserDB: the user wasn't found in the database." << std::endl;
		//create new user object
		ircUser* temp = new ircUser();
		temp->_nick= nick;

		std::cout << "posix_ircUserDB: the new nick object is at: " << std::hex << temp << std::dec << std::endl;

		registeredId_it idIter = _registeredNicksToIDs.find(nick);
		if(registered || idIter != _registeredNicksToIDs.end())
		{
			std::cout << "posix_ircUserDB: in if" << std::endl;
			temp->_userId = (*idIter).second;
		}
		else
		{
			std::cout << "posix_ircUserDB: in else" << std::endl;
			//generate a new id
			temp->_userId = getNextAvailableID();
		}
			temp->_isAuthenticated = authenticated;

		std::cout << "posix_ircUserDB: user object created, adding to database..." << std::endl;

		//add it to both maps
		_usersByNick[nick] = temp;
		_usersByID[temp->userId()] = temp;

		{
			ircUser* ohai = _usersByNick[nick];

			std::cout << "posix_ircUserDB: the new nick object in usersbynick: " << std::hex << ohai << std::dec << std::endl;
		}
	}
	else
		std::cout << "posix_ircUserDB: find didn't return map::end >.>" << std::endl;
	//else its already here so whe shouldn't need to add it
	
}

//TODO this should be redone with channels by nick
void posix_ircUserDB::removeUser(std::string nick)
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
		std::cout << "removeUser" << std::endl;
		printAllUsers();
	}

	//remove it forom channels;
	channels_it channels;
	for(channels = _usersByChannel.begin(); channels != _usersByChannel.end(); channels++)
	{
		nicklist_it nicks;
		for (nicks = (*channels).second.begin(); nicks != (*channels).second.end(); nicks++){

			//TODO figure out why this is necessary >.>
			if((*nicks) == NULL)
			{
				std::cout << "removeUser(inner loop)" << std::endl;
				printAllUsers();
				continue;
			}
			std::cout << "Given user id is: " << user->userId() << std::endl;
			std::cout << "DB user id is: " << (*nicks)->userId() << std::endl;
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

void posix_ircUserDB::addUserToChannel(std::string nick, std::string channel)
{
	std::cout << "posix_ircUserDB: adding user: " << nick << std::endl;
	std::cout << "before: " << std::endl;
	printChannelUsers(channel);
	usersByNick_it user = _usersByNick.find(nick);

	_usersByChannel[channel].push_back((*user).second);

	std::cout << "after:" << std::endl;
	printChannelUsers(channel);
}

void posix_ircUserDB::removeUserFromChannel(std::string nick, std::string channel)
{
	//look up the channel we want
	channels_it iter = _usersByChannel.find(channel);
	
	//make sure it existes
	if(iter == _usersByChannel.end())
	{
		std::cout << "posix_ircUserDB::removeUserFromChannel(): no such channel" << std::endl;
		return;
	}

	//get the vector of users
	std::vector<ircUser*> users =  (*iter).second;
	
	//find the user we want to remove from the channel in the 
	//list of nicks
	usersByNick_it temp = _usersByNick.find(nick);


	//make sure that user actually esits
	if(temp == _usersByNick.end())
	{
		std::cout << "posix_ircUserDB::removeUserFromChannel(): no such nick exists" << std::endl;
		return;
	}

	//save off that users id
	unsigned int userId = (*temp).second->userId();


	std::vector<ircUser*>::iterator channelUser;

	printChannelUsers(channel);

	//look through the vector for a pointer to a nick with the same user id.
	for(channelUser = users.begin(); channelUser != users.end(); channelUser++)
	{
		//there have been nulls in the list before 
		//handle them if we find any
		if((*channelUser) == NULL)
		{
			std::cout << "RemoveUserFromChannel" << std::endl;
			printAllUsers();
			continue;
		}

		//
		if((*channelUser)->userId() == userId)
		{
			users.erase(channelUser);
			break;
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

	//if its not then the user is gone and we need to clean up

	//remove from usersbyid
	_usersByID.erase(_usersByID.find(userId));
	
	//remove from users by nick
	_usersByNick.erase(nick);
}

void posix_ircUserDB::nickChange(std::string nick, std::string newNick)
{
	usersByNick_it it = _usersByNick.find(nick);
	ircUser* user = (*it).second;


	user->_nick = newNick;

	_usersByNick.erase(it);
	_usersByNick[newNick] = user;

}

ircUser* posix_ircUserDB::getUser(std::string nick)
{

	ircUser* user = NULL;
	usersByNick_it iter = _usersByNick.find(nick);
	if(iter != _usersByNick.end())
	{
		user = (*iter).second;
	}

	if(user == NULL)
	{
		std::cout << "getUser nick" << std::endl;
		printAllUsers();
	}
	return user;
}

ircUser* posix_ircUserDB::getUser(unsigned int ID)
{
	

	ircUser* user = NULL;
	usersByID_it iter = _usersByID.find(ID);
	if(iter != _usersByID.end())
	{
		user = (*iter).second;
	}

	if(user == NULL)
	{
		std::cout << "getUser id" << std::endl;
		printAllUsers();
	}
	return user;
}

std::vector<ircUser*> posix_ircUserDB::getChannelCurrentUsers(std::string channel)
{
	return _usersByChannel[channel];
}

std::vector<ircUser*> posix_ircUserDB::allUsers()
{
	usersByID_it iter;
	std::vector<ircUser*> users;
	for(iter = _usersByID.begin(); iter != _usersByID.end(); iter++)
	{
		users.push_back((*iter).second);
	}

	return users;
}
std::vector<ircUser*> posix_ircUserDB::onlineRegisteredUsers()
{
	registeredId_it iter;
	std::vector<ircUser*> users;
	for(iter = _registeredNicksToIDs.begin(); iter != _registeredNicksToIDs.end(); iter++)
	{
		usersByNick_it user = _usersByNick.find((*iter).first);
		if(user != _usersByNick.end())
		{
			users.push_back((*user).second);
		}
	}

	return users;

}
std::vector<ircUser*> posix_ircUserDB::allRegisteredUsers()
{
}
std::vector<ircUser*> posix_ircUserDB::authenticatedUsers()
{
	usersByID_it iter;
	std::vector<ircUser*> users;
	for(iter = _usersByID.begin(); iter != _usersByID.end(); iter++)
	{
		if((*iter).second->isAuthenticated())
		{
			users.push_back((*iter).second);
		}
	}

	return users;
}

unsigned int posix_ircUserDB::getNextAvailableID()
{

	unsigned int posId;

	/* This actually works correctly. */

	//XXX when this was a MACRO it was came back 0 >.>
//#define RAND_INV_RANGE(r) ((unsigned int) ((RAND_MAX + 1) / (r)))
	//unsigned int rand_inv_range = (unsigned int) ((RAND_MAX + 1) / RANGE);
	//do {
	    posId = (unsigned int) rand();
	//} while (posId >= RANGE * rand_inv_range);
	//posId /= rand_inv_range;

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

void posix_ircUserDB::saveData()
{
	std::cout << "posix_ircUserDB: saving data..." << std::endl;
	std::ofstream dbfile;
	dbfile.open("users.db");
	
	dbfile << _registeredNicksToIDs.size();
	std::cout << "-----Saved Data------" << std::endl;
	std::cout << "number of registered nicks: " << _registeredNicksToIDs.size() << std::endl;
	for(registeredId_it iter = _registeredNicksToIDs.begin(); iter != _registeredNicksToIDs.end(); iter++)
	{
		dbfile << iter->first;
		std::cout << "nick: " << iter->first << std::endl;
		dbfile << iter->second;
		std::cout << "id: " << iter->second << std::endl << std::endl;
	}
}

void posix_ircUserDB::loadData()
{
	std::ifstream dbfile;
	dbfile.open("users.db");

	std::cout <<"posix_ircUserDB: opening database file..." << std::endl;
	if(!dbfile.good())
		return;

	size_t mapsize;
	dbfile >> mapsize;

	std::cout <<"posix_ircUserDB: there are "<< mapsize << " registered nicks" << std::endl;

	for (size_t i = 0; i < mapsize; ++i)
	{
		std::string nick;
		dbfile >> nick;
		unsigned int id;
		dbfile >> id;
		_registeredNicksToIDs[nick] = id;

//		std::cout << "posix_ircUserDB: loaded nick: " << nick << " id: " << id << std::endl;

	}
}




void posix_ircUserDB::printAllUsers()
{
	std::stringstream users;

	users << std::endl << "-----DATABURRS DUMP-----" << std::endl;

	usersByNick_it nicks;

	for(nicks = _usersByNick.begin(); nicks!=_usersByNick.end(); nicks++)
	{
		users << (*nicks).first << "\t";

		if((*nicks).second == NULL)
		{
			users << "NULL\tNULL" << std::endl;
		}
		else
		{
			ircUser* temp =  (*nicks).second;
			users << temp->userId() << "\t";
			if(temp->isAuthenticated())
			{
				users << "true" << std::endl;
			}
			else
			{
				users << "false" <<std::endl;
			}
		}
	}


	users << std::endl << std::endl;
	usersByID_it ids;

	for(ids = _usersByID.begin(); ids!=_usersByID.end(); ids++)
	{
		users << (*ids).first << "\t";

		if((*ids).second == NULL)
		{
			users << "NULL\tNULL" << std::endl;
		}
		else
		{
			ircUser* temp =  (*ids).second;
			users << temp->nick() << "\t";
			if(temp->isAuthenticated())
			{
				users << "true" << std::endl;
			}
			else
			{
				users << "false" <<std::endl;
			}
		}
	}

	std::cout << users.str();
}

void posix_ircUserDB::printChannelUsers(std::string channel)
{
	std::vector<ircUser*> users =  _usersByChannel[channel];


	std::vector<ircUser*>::iterator channelUser;

	std::cout << "-----Users in channel " << channel << "-----" << std::endl;
	for(channelUser = users.begin(); channelUser != users.end(); channelUser++)
	{
		if((*channelUser) != NULL)
			std::cout << (*channelUser)->nick() << std::endl;
		else 
			std::cout << "NULL" << std::endl;
	}
}
