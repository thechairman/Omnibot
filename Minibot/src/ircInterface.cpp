#include "ircInterface.h"
#include <algorithm>

#include <iostream>
#include <sstream>

//parse vars

//catch nicks

//update statuses

//add timer to reconnecet if we disconnect

const std::string ircInterface::PRIVMSG = "PRIVMSG";
const std::string ircInterface::NOTICE = "NOTICE";
const std::string ircInterface::QUIT = "QUIT";
const std::string ircInterface::PING = "PING";
const std::string ircInterface::PONG = "PONG";
const std::string ircInterface::NICK = "NICK";
const std::string ircInterface::JOIN = "JOIN";
const std::string ircInterface::PART = "PART";
const std::string ircInterface::USER = "USER";
const std::string ircInterface::ERROR = "ERROR";
const std::string ircInterface::INSPIRCDVARS = "005";
const std::string ircInterface::NICKLIST = "353";

char ircInterface::CMD_DELIM = '@';
//this value may be standard for all irc severs, but my experiance is only with inspircd; joe


//helper function to print user datas
void printUser(ircUser* user)
{
	std::stringstream userdata;
	userdata << "ircInterface: User data: ";
	userdata << user->nick() << "\t";
	userdata << user->userId() << "\t";
	if(user->isAuthenticated())
		userdata <<"true";
	else
		userdata << "false";

	std::cout << userdata.str() << std::endl;

}

ircInterface::ircInterface(){
	_serverConnection = ircio::create();

	std::cout << "ircInterface: initializing user database..." << std::endl;
	_userDB = ircUserDB::create();

	std::cout << "ircINterface: initializing user authentication" << std::endl;
	_userAuth = new ircUserAuth(this, _userDB);

	_userAuth->authMethod(ircUserAuth::AUTH_NICKSERV);

	_usersInterface = new ircUsersInterface(_userDB);
}

ircInterface::~ircInterface(){
	delete _usersInterface;
	delete _userAuth;
	delete _userDB;
	delete _serverConnection;
}

int ircInterface::connect(std::string server, int port){

	_serverConnection->registerCallBack(this);
	_serverConnection->open(server, port);
	_serverConnection->sleep(SLEEP_INTRV);
	return 0;
}

int ircInterface::registerUser(std::string nick, std::string uname, std::string rname){

	std::string msg = NICK + " " + nick+ "\r\n";
	sendString(msg);
	msg = "USER " + uname + " 0 * :" + rname + "\r\n";
	sendString(msg);
	_serverConnection->sleep(SLEEP_INTRV);
	return 0;
}



int ircInterface::join(std::string channel){
	std::string msg = JOIN + " :" + channel + "\r\n";
	sendString(msg);
	return 0;
}

int ircInterface::part(std::string channel, std::string reason){
	std::string msg = "PART " + channel + " :" +reason;
	sendString(msg);
	return 0;
}


int ircInterface::part(std::string channel){
	std::string msg = "PART " + channel + "\r\n";
	sendString(msg);
	return 0;
}

int ircInterface::sendMessage(std::string channel, std::string message){
	std::cout << "Sending Message \""  << message <<"\" to channel \"" << channel <<"\"" << std::endl;

	std::string msg = "PRIVMSG " + channel + " :" + message +"\r\n";
	sendString(msg);
	return 0;
}

int ircInterface::sendPM(std::string nick, std::string message){
	std::string msg = "PRIVMSG " + nick + " :" + message + "\r\n";
	sendString(msg);
	return 0;
}

int ircInterface::quit(std::string reason){
	std::string msg = "QUIT :" + reason + "\r\n";
	sendString(msg);
	return 0;
}

int ircInterface::quit(){
	std::string msg = QUIT;
	sendString(msg);
	return 0;
}

void ircInterface::registerForNotify(ircInterfaceClient* client){
	clients.push_back(client);
	std::cout << "IrcInterface: register client for notifications" << std::endl;
}

ircUsersInterface* ircInterface::usersInterface()
{
	return _usersInterface;
}

void ircInterface::notifyEvent(ircEvent& e){
	std::vector<ircInterfaceClient*>::iterator iter;
	for(iter = clients.begin(); iter < clients.end(); iter++){
		(*iter)->alertEvent(e);
	}
}	
void ircInterface::notifyMessage(ircMessage& m){

	std::cout << "ircINterface: notifying clients of incoming messages" << std::endl;

	std::vector<ircInterfaceClient*>::iterator iter;
	for(iter = clients.begin(); iter < clients.end(); iter++){
		(*iter)->alertMessage(m);
	}
}	
void ircInterface::sendString(std::string str){
	std::cout << "ircInterface: the address of this instance: " << this <<std::endl;
	if(!_serverConnection->write(str))
		std::cout<<"oh no! string didn't send!" << std::endl;
	else
		std::cout<< "sent string to server:  "<< str << std::endl;
}

void ircInterface::onMessage(std::string pkge){
	while(pkge.find("\r\n") != std::string::npos){
		std::string msg = pkge.substr(0, pkge.find("\r\n"));
		pkge = pkge.substr(pkge.find("\r\n") + 2);

		std::cout << "ircInterface: raw message is : "<<msg<<std::endl;
		
		//alot of the control strings will start with  the type sperated from the 
		//contents of the message with a space
		std::string type = msg.substr(0, msg.find_first_of(' '));

			
		//first check for messages that start with message names
		//check for ping
		if(!type.compare(PING))
		{
			sendPong();
			return;
		}

		else if(!type.compare(ERROR))
		{	
			//need to figure out hwat to do here
			//for now lets just try and not spam the other levels
			return;
		}

		//now check for messages that start with nicks or server titles
		else 
		{	
			//type is actually a prefix containing the host mask etc
			std::string prefix = type;
			// the actual message past the prefix
			msg = msg.substr(msg.find_first_of(' ')+1);
			//the first part of that message should be the type
			type = msg.substr(0, msg.find_first_of(' '));

			//check first to see if it is a private message
			//most irc messaages are private messages
			if(!type.compare(PRIVMSG))
			{
				handle_privmsg(msg, prefix);
			}
			else if(!type.compare(NOTICE))
			{
				handle_notice(msg, prefix);
			}
			else if(!type.compare(QUIT))

			{
				ircEvent e = handle_quit(msg, prefix);
				notifyEvent(e);
			}
			else if(!type.compare(JOIN))
			{
				ircEvent e = handle_join(msg, prefix);
				notifyEvent(e);
			}
			else if(!type.compare(PART))
			{
				ircEvent e = handle_part(msg, prefix);
				notifyEvent(e);
			}

			else if(!type.compare(NICK))
			{
				ircEvent e = handle_nick(msg, prefix);
				notifyEvent(e);
			}

			else if(!type.compare(INSPIRCDVARS))
			{
				handle_vars(msg);
			}

			else if(!type.compare(NICKLIST))
			{
				//add function to parse the nicklist
				handle_nicklist(msg);
			}
		}
	}
}

void ircInterface::handle_nicklist(std::string nickMsg)
{	
	std::string channel = nickMsg.substr(nickMsg.find_first_of('=') + 2);
	channel = channel.substr(0, channel.find_first_of(' '));
	std::string nick = nickMsg.substr(nickMsg.find_first_of(':') + 1);
	
	std::cout << "ircInterface: List of nicks: " << nick << std::endl;

	std::stringstream nicks(nick);

	while(getline(nicks, nick, ' '))
	{
		std::string temp;
		temp = _userAuth->addUser(nick);
		_userDB->addUserToChannel(temp, channel);

		std::cout << "ircInterface: added nick: " << temp << " to channel: " << channel << std::endl;
	}

}
//again this entire function could be inspircd specific
void ircInterface::handle_vars(std::string varList)
{
	std::stringstream vars(varList);
	std::string assign;

	while(getline(vars, assign, ' '))
	{
		if(assign.find_first_of('=') == std::string::npos)
			continue;

		std::string var = assign.substr(0, assign.find_first_of('='));
		std::string val = assign.substr(assign.find_first_of('=') + 1);

		if(!var.compare("STATUSMSG"))
		{
			_userAuth->nickPrefixes(val);
			std::cout << "ircInterface: nick Prefixes are: " << val << std::endl;
		}
	}
}

ircEvent ircInterface::handle_quit(std::string msg, std::string prefix)
{
	std::string nick = prefix.substr(prefix.find_first_of(':') + 1, 
					 prefix.find_first_of('!') -1);

	_userDB->removeUser(nick);

	//TODO handle the quit reasons, for now its an empty string
	return ircEvent::quit(nick, "");
}

ircEvent ircInterface::handle_join(std::string msg, std::string prefix)
{
	std::string nick = prefix.substr(prefix.find_first_of(':') + 1, 
					 prefix.find_first_of('!') -1);

	//+2 because it has a : in front of the channel name
	std::string channel = msg.substr(msg.find_first_of(' ') + 2);


	_userAuth->addUser(nick);
	_userDB->addUserToChannel(nick, channel);

	return ircEvent::join(channel, nick);
}

ircEvent ircInterface::handle_part(std::string msg, std::string prefix)
{
	std::string nick = prefix.substr(prefix.find_first_of(':') + 1, 
					 prefix.find_first_of('!') -1);

	std::string channel = msg.substr(msg.find_first_of(' ') + 1);
	_userDB->removeUserFromChannel(nick, channel);

	//TODO handle part reasons for now it's an empty string
	return ircEvent::part(channel, nick, "");
}

ircEvent ircInterface::handle_nick(std::string msg, std::string prefix)
{
	
	std::string nick = prefix.substr(prefix.find_first_of(':') + 1, 
					 prefix.find_first_of('!') -1);

	std::string newNick = msg.substr(msg.find_first_of(' ') + 1);
	
	std::cout << "ircInterface: nick changed: " << nick << "->" << newNick << std::endl;


	_userDB->nickChange(nick, newNick);

	return ircEvent::nickChange(nick, newNick);
}

void ircInterface::handle_notice(std::string msg, std::string prefix){

	//grab user data
	std::string nick = prefix.substr(prefix.find_first_of(':') + 1, 
					 prefix.find_first_of('!') -1);

	//check if we are using AUTH_NICKSERV and that the message is from nickserv
	if((!nick.compare("NickServ") || !nick.compare("nickserv")) 
			&& _userAuth->authMethod() == ircUserAuth::AUTH_NICKSERV)
	{
		//send the message to the auth module

		msg = msg.substr(msg.find_first_of(':', 
					msg.find_first_of(' ', msg.find_first_of(' ') + 1) + 1) +1);
		if(msg.find("\r\n") != std::string::npos)
			msg = msg.substr(0, msg.find("\r\n"));

		_userAuth->nickServInfo(msg);
		//then we're done here
		return;
	}

	//XXX this may not be the best way to handle this...
	//so it turns out this would be a really poor way to handle this
	//apparently most of the sources of this message type would return null users :/ 
	//handle_privmsg(msg, prefix);
}

void ircInterface::handle_privmsg(std::string msg, std::string prefix){

	//grab user data
	std::string nick = prefix.substr(prefix.find_first_of(':') + 1, 
					 prefix.find_first_of('!') -1);


	//TODO this is a  temp solution that could cause hella memory leaks if not fixed later
	ircUser* temp = NULL;
	
	std::map<std::string, ircUser*>::iterator it;

	std::cout << "ircInterface: the nick were looking for is: " << nick << std::endl;

	temp = _userDB->getUser(nick);
	if(!temp)
	{
		std::cout << "ircInterface: uh-oh, this nick returned null... brace yourself for impact!" << std::endl;
	}
	
	printUser(temp);

	//TODO this should be streamlined if possible
	std::string channel =  msg.substr(msg.find_first_of(' ') + 1,
		msg.find_first_of(' ', msg.find_first_of(' ') + 1) - msg.find_first_of(' ') -1);

	//strip white space characters
	std::remove(channel.begin(), channel.end(), ' ');
	std::remove(channel.begin(), channel.end(), '\n');
	std::remove(channel.begin(), channel.end(), '\r');
	std::remove(channel.begin(), channel.end(), '\t');

	//see if its somebody or a channel
	//TODO an organized
	for(it = users.begin(); it != users.end(); it++){
		if(!(*it).first.compare(channel)){
			channel = "";
			break;
		}
	}

	//should be setting the message to just contain the message
	//TODO this should be streamlined if possible
	msg = msg.substr(msg.find_first_of(':', 
				msg.find_first_of(' ', msg.find_first_of(' ') + 1) + 1) +1);
	if(msg.find("\r\n") != std::string::npos)
	msg = msg.substr(0, msg.find("\r\n"));
	
	std::cout << "is this a command? " << msg.substr(0, msg.find_first_of(' ')) << std::endl;
	if(msg[0] == CMD_DELIM)
	{

		std::cout << "looking at minibot commands..." << std::endl;	
		if(!msg.substr(1, msg.find_first_of(' ')).compare("auth"))
		{
			_userAuth->verifyAuth(temp, msg.substr(msg.find_first_of(' ')+1 ));
			return;
		}
		else if(!msg.substr(1, msg.find_first_of(' ')).compare("users"))
		{
			_userDB->printAllUsers();
			return;
		}


	}

	ircMessage m(temp, msg, channel);

	
	notifyMessage(m);
}


void ircInterface::sendPong(){
	std::string temp = PONG + " minibot\r\n";
	sendString(temp);
}

