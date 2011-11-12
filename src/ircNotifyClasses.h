#ifndef _IRC_NOTIFY_
#define _IRC_NOTIFY_
class ircUser{
private:
	std::string _nick;
	int _userId;
public:	
	ircUser();
	ircUser(std::string nick_, int userId);
	std::string nick();
	int userId();
};

class ircMessage{
private:
	std::string _message;
	ircUser* _user;
public:
	ircMessage();
	ircMessage(ircUser* user_, std::string Message);
	std::string message();
	ircUser* user();
};

class ircEvent{

};
#endif
