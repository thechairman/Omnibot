#include <string>
#include <vector>

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "ircNotifyClasses.h"

class ircInterface{

pubilc:
	ircInterface();
	virtual ~ircInterface();
	int connect(std::string server, int port);
	int registerUser(std::string nick, std::string uname, std::string rname);
	int join(std::string channel);
	int part(std::string channel, std::string reason);
	int part(std::string channel);
	int sendMessage(std::channel, std::string message);
	int sendPM(std::string nick, std::string message);
	int quit(std::string reason);
	int quit():
	void registerForNotify(ircInterfaceClient* client);

private:
	//hit registered callbacks
	void notifyEvent(ircEvent e);
	void notifyMessage(ircMessage m);

	//called from socket handler
	void sendPong();

	//sends a string across the socket
	void sendString(std::string str);

	//handles a message received from the socket
	void handleString(std::string msg);

	//this thread just loops over the socket reading messages
	//and calling handleString when it gets one
	void receiver();

	//some kind of socket preferably boost 
	//otherwise i could always just launch
	//a thread to sit on it and handle
	//messages
	boost::asio::io_service* ios;
	boost::asio::ip::tcp::socket* sock;
	boost::thread receiverThread;
	std::vector<ircInterfaceClient*> clients;
}
