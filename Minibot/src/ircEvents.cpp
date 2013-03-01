#include "ircEvents.h"

///////////////////////////////////////////////////////
//		ircEvent_error
///////////////////////////////////////////////////////
ircEvent_error::ircEvent_error(std::string description):
	ircEvent(ET_ERROR),
	_description(description)
{
}
std::string ircEvent_error::description() const 
{
	return _description;
}



///////////////////////////////////////////////////////
//		ircEvent_join
///////////////////////////////////////////////////////
ircEvent_join::ircEvent_join(std::string nick, std::string channel):
	ircEvent(ET_CHANNEL_JOIN),
	_nick(nick),
	_channel(channel)

{
}
std::string ircEvent_join::nick() const 
{
	return _nick;
}
std::string ircEvent_join::channel() const 
{
	return _channel;
}

///////////////////////////////////////////////////////
//		ircEvent_part
///////////////////////////////////////////////////////
ircEvent_part::ircEvent_part(std::string nick, std::string channel):
	ircEvent(ET_CHANNEL_PART),
	_nick(nick),
	_channel(channel),
	_description("")
{
}
ircEvent_part::ircEvent_part(std::string nick, std::string channel, std::string description):
	ircEvent(ET_CHANNEL_PART),
	_nick(nick),
	_channel(channel),
	_description(description)
{
}
std::string ircEvent_part::nick() const 
{
	return _nick;
}
std::string ircEvent_part::channel() const 
{
	return _channel;
}
std::string ircEvent_part::description() const 
{
	return _description;
}

///////////////////////////////////////////////////////
//		ircEvent_nickChange
///////////////////////////////////////////////////////
ircEvent_nickChange::ircEvent_nickChange(std::string nick, std::string newNick):
	ircEvent(ET_NICK_CHANGE),
	_nick(nick),
	_newNick(newNick)
{
}
std::string ircEvent_nickChange::nick() const 
{
	return _nick;
}
std::string ircEvent_nickChange::newNick() const 
{
	return _newNick;
}

///////////////////////////////////////////////////////
//		ircEvent_quit
///////////////////////////////////////////////////////
ircEvent_quit::ircEvent_quit(std::string nick):
	ircEvent(ET_QUIT),
	_nick(nick),
	_description("")
{
}
ircEvent_quit::ircEvent_quit(std::string nick, std::string description):
	ircEvent(ET_QUIT),
	_nick(nick),
	_description(description)
{
}
std::string ircEvent_quit::nick() const 
{
	return _nick;
}
std::string ircEvent_quit::description() const 
{
	return _description;
}

///////////////////////////////////////////////////////
//		ircEvent_kill
///////////////////////////////////////////////////////
ircEvent_kill::ircEvent_kill(std::string nick, std::string doer):
	ircEvent(ET_KILL),
	_nick(nick),
	_doer(doer),
	_description("")
{
}
ircEvent_kill::ircEvent_kill(std::string nick, std::string doer, std::string description):
	ircEvent(ET_KILL),
	_nick(nick),
	_doer(doer),
	_description(description)
{
}
std::string ircEvent_kill::nick() const 
{
	return _nick;
}
std::string ircEvent_kill::doer() const 
{
	return _doer;
}
std::string ircEvent_kill::description() const 
{
	return _description;
}

///////////////////////////////////////////////////////
//		ircEvent_kick
///////////////////////////////////////////////////////
ircEvent_kick::ircEvent_kick(std::string nick, std::string doer):
	ircEvent(ET_KICK),
	_nick(nick),
	_doer(doer),
	_description("")
{
}
ircEvent_kick::ircEvent_kick(std::string nick, std::string doer, std::string description):
	ircEvent(ET_KICK),
	_nick(nick),
	_doer(doer),
	_description(description)
{
}
std::string ircEvent_kick::nick() const 
{
	return _nick;
}
std::string ircEvent_kick::doer() const 
{
	return _doer;
}
std::string ircEvent_kick::description() const 
{
	return _description;
}

///////////////////////////////////////////////////////
//		ircEvent_connLost
///////////////////////////////////////////////////////
ircEvent_connLost::ircEvent_connLost():
	ircEvent(ET_CONN_LOST)
{
}
