#include "ircNotifyClasses.h"
#include <iostream>
ircUser::ircUser(){
	_nick = "";
	_userId = USER_NOT_REGISTERED;
}
ircUser::ircUser(std::string nick_, int userId):_nick(nick_), _userId(userId){}

ircUser::ircUser(ircUser& user){
	_nick = user._nick;
	_userId = user._userId;
}

std::string ircUser::nick() const{
	std::string temp(_nick);
	return temp;
}

int ircUser::userId() const {
	return _userId;
}

ircMessage::ircMessage(): _message(""), _user(NULL), _channel(""){}

ircMessage::ircMessage(const ircMessage& msg): _message(msg._message), _user(msg._user),_channel(msg._channel){
	std::cout << "ircMessage: copied message size: " <<  _message.size() << std::endl;
	if(_message.size() < 500) 
		std::cout  << "ircMessage: copied message is: " << _message << std::endl;
}

ircMessage::ircMessage(ircUser* user_, std::string Message, std::string channel_){
	_user = user_;
	_message = Message;
	_channel = channel_;
}


std::string ircMessage::message() const{
	std::cout << "ircMessage: the length of the message is " << _message.size() << std::endl;
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

ircUser* ircMessage::user(){
	return _user;
}
