#include <cstdlib>
#include <iostream>

#include "OmniConfigParser.h"
#include "omnibot.h"
#include "ircLog.h"
#include "ircTypes.h"


const std::string FILENAME = "Core/omnibot.cpp";

/**
 *  Constructor that creates a new instance of irc interface when the bot is created.
 */
omnibot::omnibot():_irc(new ircInterface()), _nicks(), _manager(_irc, _nicks),_passedIrc(false){
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

/**
 *  Constructor that takes an existing ircInterface instance
 */
omnibot::omnibot(ircInterface* irc_):_irc(irc_),_nicks(),_manager(_irc, _nicks),/*_blocker(OmniBlocker::create()),*/_passedIrc(true){
	//std::cout << std::hex << &irc_ << std::dec << std::endl;
	ircLog::instance()->logf(FILENAME, "ircInteface instance: %x", &irc_);
	_irc->registerForNotify(this);
	_commands = new std::vector<OmniCommand*>();
	createCommands();
	ircLog::instance()->logf(FILENAME, "size of _commands after createCommands %u", _commands->size());
}


/**
 *  Omnibot destructor
 */
omnibot::~omnibot()
{
	if(!_passedIrc)
	{
		delete _irc;
	}

	clearCommands();
	delete _commands;
}


/**
 * Omnibot ircInterface Callback.
 * This function is called by _irc when ever a new message arrives it processes it through the
 * command classes first to see if it is directed at the omnibot or not, then passes the messages
 * to plugins for evaluation
 * @parm msg an ircMessage structure containing the message from the server
 * @return void
 */
void omnibot::alertMessage(ircMessage& msg)
{
	//std::cout << "Omnibot received string: " + toParse <<  std::endl;
	ircLog::instance()->logf(FILENAME, "Omnibot received string: %s", msg.message().c_str());

	size_t i;
	
	ircLog::instance()->logf(FILENAME, "using commands at %x", _commands);
	ircLog::instance()->logf(FILENAME, "number of commands %u", _commands->size());

	//loop over command class to identify any commands
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

	//if we went all the way through the loop and nothing happend
	//send it to the plugins
	if(i >= _commands->size())
	{
		ircLog::instance()->logf(FILENAME, "sending message to plugin mananger");
		_manager.pushMessage(msg);
	}


}


/**
 *  This function is called by ircInterface when ever an irc event occurs
 *  @parm e the event sent from _irc
 *  @return void
 */
void omnibot::alertEvent(ircEvent& e)
{
    switch(e.type())
    {
	    //if the event is caused by a lost connection
	    //we might want to try and reestablish the connection
	    case ircEvent::ET_CONN_LOST:
		{
			//get the config file and see if we are supposed to auto reconnect
			ircLog::instance()->logf(FILENAME, "Connection Lost... attempting reconnect");
			OmniConfigParser* parser = OmniConfigParser::instance();
			if(parser->autoReconnect())
			{

				//if we are supposed to try and auto reconnect try for the specified number of times
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

				//if we couldn't reconnect print an error message exit
				//theres no more good we can do here
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
/**
 *  This function goes through the proceedure to connect the ircbot to the server.
 *  This function loads the server, bot name, and default channels from the omniserverdata.cfg
 *  then establishes a connection to server, registers the user, and then joins the specified 
 *  channels
 *
 *  @return TODO
 */
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
/**
 *  This function joins the bot to the specified channel
 *  @parm channel the channel to join
 *  @return void
 */
void omnibot::join(std::string channel){
	_irc->join(channel);
}
/**
 *  This function parts the bot to the specified channel
 *  @parm channel the channel to part
 *  @return void
 */
void omnibot::part(std::string channel){
	_irc->part(channel);
}


/**
 *  This function loads the specified plugin into the bot
 *  @parm pluginName the name of the plugin to load
 *  @return true if the plugin was loaded, false if it failed
 */
bool omnibot::loadPlugin(std::string pluginName){
	return _manager.load(pluginName);
}
/**
 *  This function unloads the specified plugin
 *  @parm pluginName the name of the plugin to drop
 *  @return void
 */
void omnibot::dropPlugin(std::string pluginName){
	_manager.unload(pluginName);
}

/**
 *  Eventually this function will determine if the user has admin privileges with the bot,
 *  for naow thats everyone.
 *  @parm usr the user to look up
 *  @return returns true for every one
 */
bool omnibot::isOmniOp(ircUser& usr){
	return true;
}

/**
 * eventually this function will determine if a user is registerd and gets certian additional
 * privledges, for now thats everyone
 * @parm usr the user to look up
 * @return returns true for ever one.
 */
bool omnibot::isRegistered(ircUser& usr){
	return true;
}


/**
 * This function populates the command vector with the objects that process the different commands
 * handled by the parent bot. It configures a builder object and that object populates the vector
 * @return void
 */
void omnibot::createCommands()
{
	OmniCommandBuilder builder;

	//configure OmniCommandBuilder
	builder.setPrefix('!');
	builder.setTools(_irc, &_manager);

	ircLog::instance()->logf(FILENAME, "address of _commands = %X", _commands);

	//populate command vector
	builder.getCommands((*_commands));

	ircLog::instance()->logf(FILENAME, "size of _commands after getCommands %u", _commands->size());

}

/**
 *	This function deletes all of the commands referenced by the pointers in the command vecotr and
 *	then clears the vector.
 *	@return void
 */
void omnibot::clearCommands()
{

	ircLog::instance()->logf(FILENAME, "clearing command vector");
	for(size_t i = 0; i < _commands->size(); ++i)
	{
		delete (*_commands)[i];
	}

	_commands->clear();
}
