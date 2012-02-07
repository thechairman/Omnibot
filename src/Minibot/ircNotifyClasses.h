#ifndef _IRC_NOTIFY_
#define _IRC_NOTIFY_

#include<string>

class ircUser{

	friend class NickManager;
private:
	std::string _nick;
	bool _isAuthenticated;
	int _userId;
public:	
	enum userCodes{
		USER_NOT_REGISTERED = -1
	};
	ircUser();
	ircUser(std::string nick_, int userId = USER_NOT_REGISTERED);
	ircUser(ircUser&);
	std::string nick() const;
	int userId() const;
	bool isAuthenticated() const;
};
class ircMessage{
private:
	std::string _message;
	ircUser* _user;
	std::string _channel;
public:
	ircMessage();
	ircMessage(ircUser* user_, std::string Message, std::string channel_ = "");
	ircMessage(const ircMessage&);
	std::string message() const;
	std::string channel() const;
	bool isPrivateMsg() const;
	ircUser* user();
};

class ircEvent{

};
#endif
