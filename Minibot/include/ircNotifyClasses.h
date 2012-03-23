#ifndef _IRC_NOTIFY_
#define _IRC_NOTIFY_


#include<string>

class ircUser{

	friend class NickManager;
	friend class ircInterface;
	friend class ircUserDB;
	friend class posix_ircUserDB;
private:
	std::string _nick;
	bool _isAuthenticated;
	unsigned int _userId;
public:	
	enum userCodes{
		USER_NOT_REGISTERED = -1
	};
	ircUser();
	ircUser(std::string nick_, int userId = USER_NOT_REGISTERED);
	ircUser(const ircUser&);
	std::string nick() const;
	unsigned int userId() const;
	bool isAuthenticated() const;
};
class ircMessage{
	friend class ircInterface;
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
	friend class ircInterface;
	public:
		enum EventType{
			ET_ERROR,
			ET_CHANNEL_JOIN,
			ET_CHANNEL_PART,
			ET_NICK_CHANGE,
			ET_QUIT,
			ET_KICK,
			ET_KILL
		};

		EventType type();
		std::string channel();
		std::string nick();
		std::string newNick();
		std::string doer();
		std::string description();

	private:
		EventType _type;

		//list uses here
		//channel()
		std::string _arg1;
		

		//list uses here
		//nick()
		std::string _arg2;

		//list uses here
		//newNick()
		//description();
		std::string _arg3;
		
		//list uses here
		//doer()
		std::string _arg4;

		//put some static methods down here for creating each type of 
		//event **could perhaps make these public**
		static ircEvent error(std::string);
		static ircEvent join(std::string, std::string);
		static ircEvent part(std::string, std::string, std::string);
		static ircEvent nickChange(std::string, std::string);
		static ircEvent quit(std::string, std::string);
		static ircEvent kill(std::string, std::string, std::string);
		static ircEvent kick(std::string, std::string, std::string);
};
#endif
