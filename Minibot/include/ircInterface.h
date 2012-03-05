#ifndef _IRC_INTERFACE_
#define _IRC_INTERFACE_
#include <string>
#include <vector>
#include <map>

#include <iostream>

//#include "posix_ircio.h"
#include "ircio.h"

#include "ircInterfaceClient.h" //this includes the event, message and user classes
#include "ircUserDB.h"
#include "ircUserAuth.h"

//Frakking circular dependencies
class ircUserAuth;

class ircInterface : public ircioCallBack{

	public:
	ircInterface();
	virtual ~ircInterface();
	int connect(std::string server, int port);
	int registerUser(std::string nick, std::string uname, std::string rname);
	int join(std::string channel);
	int part(std::string channel, std::string reason);
	int part(std::string channel);
	int sendMessage(std::string channel, std::string message);
	int sendPM(std::string nick, std::string message);
	int quit(std::string reason);
	int quit();
	void registerForNotify(ircInterfaceClient* client);
	void onMessage(std::string);

	private:
	//hit registered callbacks
	void notifyEvent(ircEvent& e);
	void notifyMessage(ircMessage& m);

	//called from socket handler
	void sendPong();

	//these handle each type of message found by
	//on message
	void handle_vars(std::string);
	void handle_nicklist(std::string);
	void handle_privmsg(std::string, std::string);
	void handle_notice(std::string, std::string);
	ircEvent handle_quit(std::string, std::string);
	ircEvent handle_join(std::string, std::string);
	ircEvent handle_part(std::string, std::string );

	//sends a string across the socket
	void sendString(std::string str);

	//handles a message received from the socket
	void handleString(std::string msg);

	//TODO find a way to get this to just ircio
	ircio* _serverConnection;
	std::vector<ircInterfaceClient*> clients;

	//map to hold nicks -> user structs
	std::map<std::string, ircUser*> users;
	ircUserDB* _userDB;
	ircUserAuth* _userAuth;

	//some constants
	static const int NUM_MSG_HDRS = 2;
	static const int NUM_EVT_HDRS = 3;
	static const int SLEEP_INTRV = 7;
	//irc strings	
	static const std::string PRIVMSG;	//message header
	static const std::string NOTICE;	//message header
	static const std::string QUIT;		//event header
	static const std::string PING;
	static const std::string PONG;
	static const std::string NICK;
	static const std::string JOIN;		//event header
	static const std::string PART;		//event header
	static const std::string USER;
	static const std::string ERROR;

	//this could be inspirc specific
	static const std::string INSPIRCDVARS;
	static const std::string NICKLIST;
};

#endif
