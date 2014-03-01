#include <cstdlib>
#include <iostream>

#include "OmniConfigParser.h"
#include "omnibot.h"
#include "ircLog.h"
#include "ircTypes.h"


const std::string FILENAME = "Core/omnibot.cpp";

omnibot::omnibot():_irc(new ircInterface()), _nicks(), _manager(_irc, _nicks),/*_blocker(OmniBlocker::create()),*/_passedIrc(false){
/*	should create an irc instance here and connec
	but i don't think this is possible... you would 
	need a default server... maybe I should just take
	the empty constructor out*/

	//ircLog::instance()->logf(FILENAME, "ircInteface instance: %x", &irc_);
	_irc->registerForNotify(this);
	_commands = new std::vector<OmniCommand*>();
	createCommands();
	ircLog::instance()->logf(FILENAME, "size of _commands after createCommands %u", _commands->size());

}
omnibot::omnibot(ircInterface* irc_):_irc(irc_),_nicks(),_manager(_irc, _nicks),/*_blocker(OmniBlocker::create()),*/_passedIrc(true){
	//std::cout << std::hex << &irc_ << std::dec << std::endl;
	ircLog::instance()->logf(FILENAME, "ircInteface instance: %x", &irc_);
	_irc->registerForNotify(this);
	_commands = new std::vector<OmniCommand*>();
	createCommands();
	ircLog::instance()->logf(FILENAME, "size of _commands after createCommands %u", _commands->size());
}

omnibot::~omnibot()
{
	if(!_passedIrc)
	{
		delete _irc;
	}

	clearCommands();
	delete _commands;
}

void omnibot::alertMessage(ircMessage& msg)
{
	std::string toParse = msg.message();


	//std::cout << "Omnibot received string: " + toParse <<  std::endl;
	ircLog::instance()->logf(FILENAME, "Omnibot received string: %s", toParse.c_str());

	size_t i;
	
	ircLog::instance()->logf(FILENAME, "using commands at %x", _commands);
	ircLog::instance()->logf(FILENAME, "number of commands %u", _commands->size());

	for(i = 0; i < _commands->size(); ++i)
	{

		ircLog::instance()->logf(FILENAME, "trying command %s", (*_commands)[i]->name().c_str());

		if((*_commands)[i]->isCommandString(msg))
		{
			if((*_commands)[i]->exec(msg))
			{
				//TODO handle resultStrs;
				//probably by dumping them in the channel
			}

		}
	}

	if(i >= _commands->size())
	{
		ircLog::instance()->logf(FILENAME, "sending message to plugin mananger");
		_manager.pushMessage(msg);
	}


}

void omnibot::alertEvent(ircEvent& e)
{
    switch(e.type())
    {
	    case ircEvent::ET_CONN_LOST:
		{
			ircLog::instance()->logf(FILENAME, "Connection Lost... attempting reconnect");
			OmniConfigParser* parser = OmniConfigParser::instance();
			if(parser->autoReconnect())
			{
				ircLog::instance()->logf(FILENAME, "Attempts to reconnect availible: %d", parser->maxReconnRetries());
				int i;
				int error;
				for(i = 0, error = 0; i < parser->maxReconnRetries(); ++i)
				{

					ircLog::instance()->logf(FILENAME, "Reconnect attempt %d, %d attempts remaining", 
							i+1, parser->maxReconnRetries() - i + 1);
					error = connect();
					if(!error)
					{
						break;
					}

					//some kind of sleep here.
				}

				if( i == parser->maxReconnRetries() )
				{
					ircLog::instance()->logf(FILENAME, "Could not reconnect to server %d", error);
					std::cerr << "failed to reconnect: error " << error << std::endl;
					exit (1);
				}

			}
			else
			{
				ircLog::instance()->logf(FILENAME, "Auto Reconnections not enabled (probably should exit here)");
			}
		}
    }

}
int omnibot::connect()
{
	int rc = 0;

	ircLog::instance()->logf(FILENAME, "size of _commands in connect() %u", _commands->size());
	OmniConfigParser* parser = OmniConfigParser::instance();
	parser->parse();

	//std::cout << "serverName = " << parser->serverName() << std::endl;
	//std::cout << "serverPort = " << parser->serverPort() << std::endl;
	//std::cout << "nick = " << parser->nick() << std::endl;
	ircLog::instance()->logf(FILENAME, "serverName = %s", parser->serverName().c_str() );
	ircLog::instance()->logf(FILENAME, "serverPort = %d", parser->serverPort() );
	ircLog::instance()->logf(FILENAME, "nick = %s", parser->nick().c_str() );

	//try to connect; test if successful
	rc = _irc->connect(parser->serverName(), parser->serverPort());

	if(rc)
	{
		return rc;
	}


	//try to register with the bot's primary name; test for success
	rc = _irc->registerUser(parser->nick(), parser->nick(), parser->nick());

	if(rc == NICK_COLLISION)
	{
		//TODO add alternate nick to parameters
		return rc;
	}
	else if(rc)
	{
		return rc;
	}


	std::vector<std::string> channels = parser->channels();

	for(size_t i = 0; i < channels.size(); ++i)
	{
		_irc->join(channels[i]);
	
	}

	std::vector<std::string> plugins = parser->plugins();

	for(size_t i = 0; i < plugins.size(); ++i)
	{
		bool plugin_rc = _manager.load(plugins[i]);
		if(plugin_rc)
			ircLog::instance()->logf(FILENAME, "succefully loaded plugin %s on connect", plugins[i].c_str());
		else
			ircLog::instance()->logf(FILENAME, "failed to load plugin %s on connect", plugins[i].c_str());
	
	}

	return rc;
}
void omnibot::exec()
{
	/*_blocker->block();*/
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

void omnibot::createCommands()
{
	OmniCommandBuilder builder;

	builder.setPrefix('!');
	builder.setTools(_irc, &_manager);

	ircLog::instance()->logf(FILENAME, "address of _commands = %X", _commands);

	builder.getCommands((*_commands));

	ircLog::instance()->logf(FILENAME, "size of _commands after getCommands %u", _commands->size());

}
void omnibot::clearCommands()
{

	ircLog::instance()->logf(FILENAME, "clearing command vector");
	for(size_t i = 0; i < _commands->size(); ++i)
	{
		delete (*_commands)[i];
	}

	_commands->clear();
}
