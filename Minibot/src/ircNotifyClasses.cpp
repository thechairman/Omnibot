#include "ircNotifyClasses.h"
#include <iostream>
ircUser::ircUser(){
	_nick = "";
	_userId = USER_NOT_REGISTERED;
}
ircUser::ircUser(std::string nick_, int userId):_nick(nick_), _userId(userId){}

ircUser::ircUser(const ircUser& user){
	_nick = user._nick;
	_userId = user._userId;
}

std::string ircUser::nick() const{
	//std::cout << "nick length is: " << _nick.size() << " nick string " << _nick << std::endl;
	std::string temp(_nick);
	return temp;
}

unsigned int ircUser::userId() const {
	return _userId;
}
bool ircUser::isAuthenticated() const
{
	return _isAuthenticated;
}

ircMessage::ircMessage(): _message(""), _user(), _channel(""){}

ircMessage::ircMessage(const ircMessage& msg): _message(msg._message), _user(msg._user),_channel(msg._channel){
	//std::cout << "ircMessage: copied message size: " <<  _message.size() << std::endl;
	//if(_message.size() < 500) 
	//	std::cout  << "ircMessage: copied message is: " << _message << std::endl;
}

ircMessage::ircMessage(ircUser* user_, std::string Message, std::string channel_):_user(*user_){
//	_user = user_;
	_message = Message;
	_channel = channel_;
}


std::string ircMessage::message() const{
	//std::cout << "ircMessage: the length of the message is " << _message.size() << std::endl;
	//std::cout << "ircMessage trying to return message: " << _message << std::endl;
	std::string temp(_message);
	return temp;
}

std::string ircMessage::channel() const {
	std::string temp(_channel);
	return temp;
}

bool ircMessage::isPrivateMsg() const{
	if (!_channel.compare(""))
		return true;
	else
		return false;

}

ircUser& ircMessage::user(){
	return _user;
}

ircEvent::ircEvent(ircEvent::EventType e):_type(e){}

ircEvent::EventType ircEvent::type(){
	return _type;
}

/*std::string ircEvent::channel(){
	if(_type != ET_CHANNEL_JOIN && _type != ET_CHANNEL_PART)
		return "INVALID";
	else 
		return _arg1;
}

std::string ircEvent::nick(){
	if(_type != ET_CHANNEL_JOIN && _type != ET_CHANNEL_PART &&_type != ET_NICK_CHANGE && _type != ET_QUIT && _type != ET_KICK && _type != ET_KILL )
		return "INVALID";
	else 
		return _arg2;
}

std::string ircEvent::newNick(){
	if(_type != ET_NICK_CHANGE)
		return "INVALID";
	else
		return _arg3;
}

std::string ircEvent::doer(){
	if( _type != ET_KICK && _type != ET_KILL )
		return "INVALID";
	else 
		return _arg4;
}

std::string ircEvent::description(){
	if(_type != ET_CHANNEL_PART && _type != ET_ERROR && _type != ET_QUIT && _type != ET_KICK && _type != ET_KILL )	
		return "INVALID";
	else 
		return _arg3;

}


//static functions for creating events
ircEvent ircEvent::error(std::string description){
	ircEvent e;
	e._type = ET_ERROR;
	e._arg3 = description;
	return e;
}

ircEvent ircEvent::join(std::string channel, std::string nick){
	ircEvent e;
	e._type = ET_CHANNEL_JOIN;
	e._arg1 = channel;
	e._arg2 = nick;
	return e;
}

ircEvent ircEvent::part(std::string channel, std::string nick, std::string reason){
	ircEvent e;
	e._type = ET_CHANNEL_PART;
	e._arg1 = channel;
	e._arg2 = nick;
	e._arg3 = reason;
	return e;

}

ircEvent ircEvent::nickChange(std::string nick, std::string newNick){
	ircEvent e;
	e._type = ET_NICK_CHANGE;
	e._arg2 = nick;
	e._arg3 = newNick;
	return e;

}

ircEvent ircEvent::quit(std::string nick, std::string reason){
	ircEvent e;
	e._type = ET_QUIT;
	e._arg2 = nick;
	e._arg3 = reason;
	return e;
}

ircEvent ircEvent::kill(std::string nick, std::string doer, std::string reason){
	ircEvent e;
	e._type = ET_KILL;
	e._arg2 = nick;
	e._arg3 = reason;
	e._arg4 = doer;
	return e;

}
ircEvent ircEvent::kick(std::string nick, std::string doer, std::string reason){
	ircEvent e;
	e._type = ET_KICK;
	e._arg2 = nick;
	e._arg3 = reason;
	e._arg4 = doer;
	return e;

}
ircEvent ircEvent::connectionLost(){
	ircEvent e;
	e._type = ET_CONN_LOST;
}
*/
