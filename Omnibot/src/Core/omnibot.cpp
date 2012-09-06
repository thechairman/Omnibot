#include <iostream>

#include "OmniConfigParser.h"
#include "omnibot.h"
#include "ircLog.h"


const std::string FILENAME = "Core/omnibot.cpp";

omnibot::omnibot():_irc(new ircInterface()), _nicks(), _manager(_irc, _nicks),_blocker(OmniBlocker::create()),_passedIrc(false){
/*	should create an irc instance here and connec
	but i don't think this is possible... you would 
	need a default server... maybe I should just take
	the empty constructor out*/

	//ircLog::instance()->logf(FILENAME, "ircInteface instance: %x", &irc_);
	_irc->registerForNotify(this);

}
omnibot::omnibot(ircInterface* irc_):_irc(irc_),_nicks(),_manager(_irc, _nicks),_blocker(OmniBlocker::create()),_passedIrc(true){
	//std::cout << std::hex << &irc_ << std::dec << std::endl;
	ircLog::instance()->logf(FILENAME, "ircInteface instance: %x", &irc_);
	_irc->registerForNotify(this);
}

omnibot::~omnibot()
{
	if(!_passedIrc)
	{
		delete _irc;
	}
}

void omnibot::alertMessage(ircMessage& msg)
{
	std::string toParse = msg.message();


	//std::cout << "Omnibot received string: " + toParse <<  std::endl;
	ircLog::instance()->logf(FILENAME, "Omnibot received string: %s", toParse.c_str());

	//parse omnibot commands
	//may just want to prefix omnibot
	//or both...
	if(toParse.at(0) == '!'){

		//should probably first see if its an omniop
		//before doing any of this...
		if(!isOmniOp(msg.user())){
			//tell them omnibot only list
			_irc->sendMessage(msg.channel(), msg.user().nick() + ": I only listen to OmniOps :P");
		}

		//std::cout << "omnibot: " << toParse << std::endl;

		if (!toParse.substr(1, 4).compare("load")){
			if(loadPlugin(toParse.substr(6)))
			{
				_irc->sendMessage(msg.channel(), toParse.substr(6) + " loaded...");
			}
			else
			{
				_irc->sendMessage(msg.channel(), toParse.substr(6) + " failed to load :(");

			}
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
		else if(!toParse.substr(1,4).compare("quit"))
		{
			_irc->quit();
			//TODO need to notify main() that we're done here
			//a counting semaphore would do this nicely, but
			//have to be wrapped in a class so that we could 
			//abstract away the machine specifc aspects.
			_blocker->release();
			exit(0);
		}
		else if(!toParse.substr(1,4).compare("part"))
		{
			part(toParse.substr(6));
		}
		else if(!toParse.substr(1,4).compare("list"))
		{
			_irc->sendMessage(msg.channel(), "Loaded Plugins: " + _manager.listLoadedPlugins());
		}
		else
		{
			//pass string to plugins
			//std::cout << "Omnibot: sending message to plugin manager" << std::endl;
			ircLog::instance()->logf(FILENAME, "sending message to plugin manager");
			_manager.pushMessage(msg);
		}


	}
	else
	{
		//pass string to plugin Manager
		//std::cout << "Omnibot: sending message to plugin manager" << std::endl;
		ircLog::instance()->logf(FILENAME, "sending message to plugin manager");
		_manager.pushMessage(msg);
	}
}

void omnibot::alertEvent(ircEvent& e){}
void omnibot::connect()
{

	OmniConfigParser* parser = OmniConfigParser::instance();
	parser->parse();

	//std::cout << "serverName = " << parser->serverName() << std::endl;
	//std::cout << "serverPort = " << parser->serverPort() << std::endl;
	//std::cout << "nick = " << parser->nick() << std::endl;
	ircLog::instance()->logf(FILENAME, "serverName = %s", parser->serverName().c_str() );
	ircLog::instance()->logf(FILENAME, "serverPort = %d", parser->serverPort() );
	ircLog::instance()->logf(FILENAME, "nick = %s", parser->nick().c_str() );

	_irc->connect(parser->serverName(), parser->serverPort());
	_irc->registerUser(parser->nick(), parser->nick(), parser->nick());

	std::vector<std::string> channels = parser->channels();

	for(size_t i = 0; i < channels.size(); ++i)
	{
		_irc->join(channels[i]);
	
	}
}
void omnibot::exec()
{
	_blocker->block();
}
void omnibot::join(std::string channel){
	_irc->join(channel);
}
void omnibot::part(std::string channel){
	_irc->part(channel);
}

bool omnibot::loadPlugin(std::string pluginName){
	return _manager.load(pluginName);
}
void omnibot::dropPlugin(std::string pluginName){
	_manager.unload(pluginName);
}

bool omnibot::isOmniOp(ircUser& usr){
	return true;
}

bool omnibot::isRegistered(ircUser& usr){
	return true;
}
