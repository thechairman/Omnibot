#include "ircInterface.h"


#include <iostream>
const std::string ircInterface::PRIVMSG = "PRIVMSG";
const std::string ircInterface::QUIT = "QUIT";
const std::string ircInterface::PING = "PING";
const std::string ircInterface::PONG = "PONG";
const std::string ircInterface::NICK = "NICK";
const std::string ircInterface::JOIN = "JOIN";
const std::string ircInterface::PART = "PART";
const std::string ircInterface::USER = "USER";
const std::string ircInterface::ERROR = "ERROR";

ircInterface::ircInterface(){
	
}

ircInterface::~ircInterface(){
}

int ircInterface::connect(std::string server, int port){

	serverConnection.registerCallBack(this);
	serverConnection.open(server, port);
	serverConnection.sleep(SLEEP_INTRV);
	return 0;
}

int ircInterface::registerUser(std::string nick, std::string uname, std::string rname){

	std::string msg = NICK + " " + nick+ "\r\n";
	sendString(msg);
	msg = "USER " + uname + " 0 * :" + rname + "\r\n";
	sendString(msg);
	serverConnection.sleep(SLEEP_INTRV);
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

void ircInterface::notifyEvent(ircEvent e){
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
	if(!serverConnection.write(str))
		std::cout<<"oh no! string didn't send!" << std::endl;
	else
		std::cout<< "sent string to server:  "<< str << std::endl;
}

void ircInterface::onMessage(std::string msg){
	std::cout << msg << std::endl;
	std::string type = msg.substr(0, msg.find_first_of(' '));
	//std::cout <<msg <<std::endl;
	
	//first check for messages that start with message names
	//check for ping
	if(!type.compare(PING)){
		sendPong();
		return;
	}

	if(!type.compare(ERROR)){	
		//need to figure out hwat to do here
		//for now lets just try and not spam the other levels
		return;
	}	
	
	//then ones that start with nicks
	std::string prefix = type;
	msg = msg.substr(msg.find_first_of(' ')+1);
	type = msg.substr(0, msg.find_first_of(' '));
	
	//then other things

	//check for message
	if(!type.compare(PRIVMSG)){
		//grab user data
		std::string nick = prefix.substr(prefix.find_first_of(':') + 1, prefix.find_first_of('!') -1);

		//TODO this is just a temporary solution that could cause hella memory leaks if not fixed later
		ircUser* temp = NULL;
		
		std::map<std::string, ircUser*>::iterator it;

		for(it = users.begin(); it != users.end(); it++){
			if(!(*it).first.compare(nick)){
				temp = (*it).second;
				break;
			}
		}

		if(temp == NULL){
			temp = new ircUser(nick);
			users[nick] = temp;
		}

		//TODO this should be streamlined if possible
		std::string channel =  msg.substr(msg.find_first_of(' ') + 1, msg.find_first_of(' ', msg.find_first_of(' ') + 1) - msg.find_first_of(' ') -1);
		std::remove(channel.begin(), channel.end(), ' ');
		std::remove(channel.begin(), channel.end(), '\n');
		std::remove(channel.begin(), channel.end(), '\r');
		std::remove(channel.begin(), channel.end(), '\t');

		for(it = users.begin(); it != users.end(); it++){
			if(!(*it).first.compare(channel)){
				channel = "";
				break;
			}
		}

		//should be setting the message to just contain the message
		//TODO this should be streamlined if possible
		msg = msg.substr(msg.find_first_of(':', msg.find_first_of(' ', msg.find_first_of(' ') + 1) + 1) +1);
		if(msg.find("\r\n") != std::string::npos)
			std::cout << "holy hell! its got newlines" <<std::endl;
		msg = msg.substr(0, msg.find("\r\n"));
		//std::remove(msg.begin(), msg.end(), '\n');
		//std::remove(msg.begin(), msg.end(), '\r');
		//std::remove(msg.begin(), msg.end(), '\t');
		ircMessage m(temp, msg, channel);

		
		notifyMessage(m);
	}
	
	//check for action
	if(!type.compare(QUIT) ||
			!type.compare(JOIN)||
			!type.compare(PART))
	{
		//ircEvent e;

		//build event here
		
		//notifyEvent(e);
	}
}

void ircInterface::sendPong(){
	std::string temp = PONG + " minibot\r\n";
	sendString(temp);
}
