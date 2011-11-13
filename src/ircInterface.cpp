#include "ircInterface.h"

const std::string ircInterface::PRIVMSG = "PRIVMSG";
const std::string ircInterface::QUIT = "QUIT";
const std::string ircInterface::PING = "PING";
const std::string ircInterface::PONG = "PONG";
const std::string ircInterface::NICK = "NICK";
const std::string ircInterface::JOIN = "JOIN";
const std::string ircInterface::PART = "PART";
const std::string ircInterface::USER = "USER";


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
	std::string msg = "PART " + channel;
	sendString(msg);
	return 0;
}

int ircInterface::sendMessage(std::string channel, std::string message){
	std::string msg = "PRIVMSG " + channel + " :" + message;
	sendString(msg);
	return 0;
}

int ircInterface::sendPM(std::string nick, std::string message){
	std::string msg = "PRIVMSG " + nick + " :" + message;
	sendString(msg);
	return 0;
}

int ircInterface::quit(std::string reason){
	std::string msg = "QUIT :" + reason;
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
}

void ircInterface::notifyEvent(ircEvent e){
	std::vector<ircInterfaceClient*>::iterator iter;
	for(iter = clients.begin(); iter < clients.end(); iter++){
		(*iter)->alertEvent(e);
	}
}	
void ircInterface::notifyMessage(ircMessage m){
	std::vector<ircInterfaceClient*>::iterator iter;
	for(iter = clients.begin(); iter < clients.end(); iter++){
		(*iter)->alertMessage(m);
	}
}	
void ircInterface::sendString(std::string str){
	serverConnection.write(str);
	std::cout << str << std::endl;
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
	
	//then ones that start with nicks
	std::string prefix = type;
	msg = msg.substr(msg.find_first_of(' ')+1);
	type = msg.substr(0, msg.find_first_of(' '));
	
	//then other things

	//check for message
	if(!type.compare(PRIVMSG)){
		//ircMessage m;

		//build message here
		
		//notifyMessage(m);
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
