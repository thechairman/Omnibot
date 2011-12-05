
#include "omnibot.h"

/*omnibot::omnibot(){
	should create an irc instance here and connec
	but i don't think this is possible... you would 
	need a default server... maybe I should just take
	the empty constructor out
}*/
omnibot::omnibot(ircInterface& irc_):_irc(irc_),_nicks(),_manager(_irc, _nicks){
}

void omnibot::alertMessage(ircMessage msg)
{
	std::string toParse = msg.message();

	//parse omnibot commands
	//may just want to prefix omnibot
	//or both...
	if(toParse.at(0) == '!'){

		//should probably first see if its an omniop
		//before doing any of this...
		if(!isOmniOp(msg.user())){
			//tell them omnibot only list
			_irc.sendMessage(msg.channel(), msg.user()->nick() + ": I only listen to OmniOps :P");
		}

		if (!toParse.substr(1, 4).compare("load")){
			loadPlugin(toParse.substr(6));
		}
		else if(!toParse.substr(1, 4).compare("drop"))
		{
			dropPlugin(toParse.substr(6));
		}
		else if(!toParse.substr(1,2).compare("op"))
		{

		}
		else if(!toParse.substr(1,4).compare("unop"))
		{

		}
		else if(!toParse.substr(1,4).compare("join"))
		{
			join(toParse.substr(6));
		}
		else if(!toParse.substr(1,4).compare("part"))
		{
			part(toParse.substr(6));
		}
		else
		{
			//pass string to plugins
			_manager.pushMessage(msg);
		}


	}
	else
	{
		//pass string to plugin Manager
		_manager.pushMessage(msg);
	}
}

void omnibot::alertEvent(ircEvent e){}
void omnibot::join(std::string channel){
	_irc.join(channel);
}
void omnibot::part(std::string channel){
	_irc.part(channel);
}

void omnibot::loadPlugin(std::string pluginName){
	_manager.load(pluginName);
}
void omnibot::dropPlugin(std::string pluginName){
	_manager.unload(pluginName);
}

bool omnibot::isOmniOp(ircUser* usr){
	return true;
}

bool omnibot::isRegistered(ircUser* usr){
	return true;
}
