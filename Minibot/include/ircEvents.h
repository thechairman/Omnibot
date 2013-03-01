#include "ircNotifyClasses.h"

/*
enum EventType{
ET_ERROR,
ET_CHANNEL_JOIN,
ET_CHANNEL_PART,
ET_NICK_CHANGE,
ET_QUIT,
ET_KICK,
ET_KILL,

ET_CONN_LOST
};

*/

class ircEvent_error:public ircEvent
{
	public:
		ircEvent_error(std::string);
		std::string description() const;

	private:
		std::string _description;
};

class ircEvent_join:public ircEvent
{
	public:
		ircEvent_join(std::string, std::string);	
		std::string nick() const;
		std::string channel() const;

	private:
		std::string _nick;
		std::string _channel;
};
class ircEvent_part:public ircEvent
{
	public:
		ircEvent_part(std::string, std::string);
		ircEvent_part(std::string, std::string, std::string);
		std::string nick() const;
		std::string channel() const;
		std::string description() const;

	private:
		std::string _nick;
		std::string _channel;
		std::string _description;
};
class ircEvent_nickChange:public ircEvent
{
	public:
		ircEvent_nickChange(std::string, std::string);	
		std::string nick() const;
		std::string newNick() const;

	private:
		std::string _nick;
		std::string _newNick;
};
class ircEvent_quit:public ircEvent
{
	public:
		ircEvent_quit(std::string);
		ircEvent_quit(std::string, std::string);	
		std::string nick() const;
		std::string description() const;

	private:
		std::string _nick;
		std::string _description;
};
class ircEvent_kill:public ircEvent
{
	public:
		ircEvent_kill(std::string, std::string);
		ircEvent_kill(std::string, std::string, std::string);
		std::string doer() const;
		std::string nick() const;
		std::string description() const;

	private:
		std::string _nick;
		std::string _doer;
		std::string _description;
};
class ircEvent_kick:public ircEvent
{
	public:
		ircEvent_kick(std::string, std::string);
		ircEvent_kick(std::string, std::string, std::string);
		std::string doer() const;
		std::string nick() const;
		std::string description() const;

	private:
		std::string _nick;
		std::string _doer;
		std::string _description;
};
class ircEvent_connLost:public ircEvent
{
	public:
		ircEvent_connLost();
};
