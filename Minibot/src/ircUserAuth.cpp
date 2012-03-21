#include "ircUserAuth.h"

ircUserAuth::ircUserAuth(ircInterface* irc, ircUserDB* db):_userDB(db),_irc(irc),_nickModePrefixes("~&@%+-"){}
ircUserAuth::~ircUserAuth(){}

void ircUserAuth::nickServInfo(std::string str)
{
	//find ways to determin response
	if(!str.substr(0, str.find_first_of(' ')).compare("STATUS"))
	{	
		//its a status message
		str = str.substr(str.find_first_of(' ') + 1);
		std::string nick = str.substr(0, str.find_first_of(' '));
		std::string status = str.substr(str.find_first_of(' ') + 1, 1);

		if(!status.compare("3") || !status.compare("2"))
		{
			//_userDB->addUser(nick, true, true);
			_userDB->setRegistered(nick, true);
			_userDB->setAuthenticated(nick, true);
		}
		else if(!status.compare("1"))
		{
			//_userDB->addUser(nick, true, false);
			_userDB->setRegistered(nick, true);
			_userDB->setAuthenticated(nick, false);
		}
		else if(!status.compare("0"))
		{
			//_userDB->addUser(nick, false, false);
			_userDB->setRegistered(nick, false);
			_userDB->setAuthenticated(nick, false);
		}

	}
}

std::string ircUserAuth::addUser(std::string nick)
{
	//need to strip leading symbols if any...
	if(_nickModePrefixes.find(nick[0]) != std::string::npos)
		nick = nick.substr(1);

	//check to see if this user exists and is authenticated
	ircUser* usr = _userDB->getUser(nick);
	if(usr != NULL && usr->isAuthenticated())
		return nick;
	
	if(_authMethod == AUTH_NICKSERV)
	{
		std::cout << "ircUserAuth: calling ircUserDB::addUser()" << std::endl;
		_userDB->addUser(nick, false, false);


		std::cout << "ircUserAuth: pinging nickserve" << std::endl;
		//if it doesn't exist or isn't authenticated run a status on nickserve
		_irc->sendPM("Nickserv", "status " + nick);

	}

	return nick;
	
}
void ircUserAuth::authMethod(ircUserAuth::AuthType type)
{
	 _authMethod = type;
}

ircUserAuth::AuthType ircUserAuth::authMethod()
{
	return _authMethod;
}

void ircUserAuth::nickPrefixes(std::string prefixes)
{
	_nickModePrefixes = prefixes;
}

void ircUserAuth::verifyAuth(ircUser* user, std::string credentials)
{
	if(_authMethod == AUTH_NICKSERV)
	{
		std::cout << "ircUserAuth: pinging nickserve" << std::endl;
		//if it doesn't exist or isn't authenticated run a status on nickserve
		_irc->sendPM("Nickserv", "status " + user->nick());
	}
}
