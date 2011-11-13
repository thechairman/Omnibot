#include <string>
#include <vector>
#include <map>

#include <iostream>

#include "posix_ircio.h"
#include "ircInterfaceClient.h"

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
	void notifyEvent(ircEvent e);
	void notifyMessage(ircMessage m);

	//called from socket handler
	void sendPong();

	//sends a string across the socket
	void sendString(std::string str);

	//handles a message received from the socket
	void handleString(std::string msg);

	posix_ircio serverConnection;
	std::vector<ircInterfaceClient*> clients;

	//map to hold nicks -> user structs
	std::map<std::string, ircUser*> users;

	//some constants
	static const int NUM_MSG_HDRS = 1;
	static const int NUM_EVT_HDRS = 3;
	static const int SLEEP_INTRV = 7;
	//irc strings	
	static const std::string PRIVMSG;	//message header
	static const std::string QUIT;		//event header
	static const std::string PING;
	static const std::string PONG;
	static const std::string NICK;
	static const std::string JOIN;		//event header
	static const std::string PART;		//event header
	static const std::string USER;
};
