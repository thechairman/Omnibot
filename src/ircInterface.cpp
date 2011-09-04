#include "ircInterface.h"
ircInterface::ircInterface(){
	ios = new boost::asio::io_service;
	sock = new boost::asio::ip::tcp::socket(*ios);
	
}

ircInterface::~ircInterface(){
	//should probably come up with a way to 
	//make sure these ahve been closed and flushed first
	delete ios;
	delete sock;
}

int ircInterface::connect(std::string server, int port){
	boost::asio::ip::tcp::resolver resolver(*ios);
	boost::aso::ip::tcp::resolver::query query(&server, "irc");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::connect(*sock, endpoint_iterator);

	//should probably launch a thread to listen to the socket here
	receiverThread = new boost::thread(&ircInterface::receiver, this);
	return 0;
}

int ircInterface::registerUser(std::string nick, std::string uname, std::string rname){

	std::string msg = "NICK " + nick;
	sendString(msg);
	msg = "USER " + uname + " 0 * :" + rname;
	sendString(msg);
	return 0;
}

int ircInterface::join(std::string channel){
	std::string msg = "JOIN :"+channel;
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
	std::string msg = "QUIT";
	sendString(msg);
	return 0;
}

void registerForNotify(ircInterfaceClient* client){
	clients.push_back(client);
}

void ircInterface::notifyEvent(ircEvent e){
	std::vector<ircInerfaceClient*>::iterator iter;
	for(iter = clients.begin(); it < clients.end(); iter++){
		(*iter)->alertEvent(e);
	}
}	
void ircInterface::notifyMessage(ircMessage m){
	std::vector<ircInerfaceClient*>::iterator iter;
	for(iter = clients.begin(); it < clients.end(); iter++){
		(*iter)->alertMessage(m);
	}
}	
void ircInterface::sendString(std::string str){
	std::cout << str << std::endl;
}

void irc
