#include <fstream>
#include <algorithm>
#include <iostream>

#include "OmniConfigParser.h"

#include "ircLog.h"

const std::string FILENAME = "OmniConfigParser.cpp";

const std::string OmniConfigParser::OMNI_CONFIG_FILE_NAME = "omniserverdata.cfg";

#if 0
const std::string OmniConfigParser::NickSection::NICKS = "nick";
const std::string OmniConfigParser::SERVER = "server";
const std::string OmniConfigParser::CHANNELS = "channels";
const std::string OmniConfigParser::PLUGINS = "plugins";
const std::string OmniConfigParser::AUTHENTICATION = "authentication";
const std::string OmniConfigParser::RECONNECTIONS = "reconnections";
const std::string OmniConfigParser::PLUGIN_LOADING = "plugin loading"
#endif

const std::string OmniConfigParser::NickSection::NickSection::THIS_SECTION = "nick";
const std::string OmniConfigParser::ServerSection::THIS_SECTION = "server";
const std::string OmniConfigParser::ChannelsSection::THIS_SECTION = "channels";
const std::string OmniConfigParser::PluginsSection::THIS_SECTION = "plugins";
const std::string OmniConfigParser::AuthSection::THIS_SECTION = "authentication";
const std::string OmniConfigParser::ReconnSection::THIS_SECTION = "reconnections";

const std::string 	OmniConfigParser::NickSection::DEFAULT_NICK = "Omnibot";
const std::string 	OmniConfigParser::ServerSection::DEFAULT_SERVER = "";
const int 		OmniConfigParser::ServerSection::DEFAULT_PORT = 6667;
const bool 		OmniConfigParser::ReconnSection::DEFAULT_AUTOCONNECT = false;
const int  		OmniConfigParser::ReconnSection::DEFAULT_RETRIES = 0;
const std::string 	OmniConfigParser::PluginsSection::DEFAULT_PLUGIN_DIR = "../plugins" ;
const OmniConfigParser::pluginLoadModes 	OmniConfigParser::PluginsSection::DEFAULT_LOAD_MODE = PLM_STATIC;


OmniConfigParser* OmniConfigParser::_instance = NULL;

std::string OmniConfigParser::nick()
{
	return _nick;
}
std::string OmniConfigParser::serverName()
{
	return _serverName;
}
int OmniConfigParser::serverPort()
{
	return _serverPort;
}
std::vector<std::string> OmniConfigParser::channels()
{
	return _channels;
}

std::vector<std::string> OmniConfigParser::plugins()
{
	return _plugins;
}
bool OmniConfigParser::autoReconnect()
{
	return _autoReconnect;
}
int OmniConfigParser::maxReconnRetries()
{
	return _maxReconnRetries;
}
OmniConfigParser::pluginLoadModes OmniConfigParser::pluginLoadMode()
{
	return _pluginLoadMode;
}
std::string OmniConfigParser::pluginDirectory()
{
	return _pluginDir;
}
#if 0
int  OmniConfigParser::parse()
{
	std::ifstream configFile(OMNI_CONFIG_FILE_NAME.c_str());
	if(configFile.fail())
	{
		return P_IO_ERROR;
	}
	std::string line;
	int state = PS_INVALID;
	while(!configFile.eof())
	{
		getline(configFile, line);
		
		if(!line.compare("") || !line.compare("\n"))
			continue;

		//skip commented lines
		if(line[0] == '#')
		{
			continue;
		}

		//catch section delimeter lines
		if(line[0] == '[')
		{
			size_t start, end;

			//skip the first char so + 1
			//since we +1 we -1 from the end
			start = line.find_first_of('[') + 1;
			end = line.find_first_of(']') - 1;

			std::string section = line.substr(start, end);

			if(!section.compare(NICKS))
			{
				state = PS_NICK;
			}
			else if(!section.compare(SERVER))
			{
				state = PS_SERVER;
			}
			else if(!section.compare(CHANNELS))
			{
				state = PS_CHANNELS;
			}
			else if(!section.compare(PLUGINS))
			{
				state = PS_PLUGINS;
			}
			else if(!section.compare(AUTHENTICATION))
			{
				state = PS_AUTHENTICATION;
			}
			else if(!section.compare(RECONNECTIONS))
			{
				ircLog::instance()->logf(FILENAME, "In reconnection section");
				state = PS_RECONNECTION;
			}
			else if(!section.compare(PLUGIN_LOADING))
			{
				state = PS_PLUGIN_LOADING;
			}
			else
			{
				return P_INVALID_SECTION;
			}

			//this is a section line
			continue;
		}

		switch(state)
		{
			case PS_NICK:
			{
				size_t end;
				end = line.find_first_of('=');
				_nick = line.substr(end + 2);
				continue;
			}
			case PS_SERVER:
			{
				size_t start, end;
				start = 0;
				end = line.find_first_of('=');
				std::string field = line.substr(start, end - 1);
				if(!field.compare("name"))
				{
					_serverName = line.substr(end + 2);
				}
				else if(!field.compare("port"))
				{
					_serverPort = atoi(line.substr(end+2).c_str());
				}
				else
				{
					return P_INVALID_FIELD;
				}
				continue;
			}
			case PS_CHANNELS:
			{
				size_t end;
				end = line.find_first_of('=');
				//get string
				line =  line.substr(end + 2);
				ircLog::instance()->logf(FILENAME, "channel before stripping = %s", line.c_str());
				//end = line.size();
				line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
				//line = line.substr(0, end - 2);
				ircLog::instance()->logf(FILENAME, "channel after stripping = %s", line.c_str());
				_channels.push_back(line);
				continue;
			}
			case PS_PLUGINS:
			{
				size_t end;
				end = line.find_first_of('=');
				//get string
				line =  line.substr(end + 2);
				ircLog::instance()->logf(FILENAME, "plugin before stripping = %s", line.c_str());
				//end = line.size();
				line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
				//line = line.substr(0, end - 2);
				ircLog::instance()->logf(FILENAME, "plugin after stripping = %s", line.c_str());
				_plugins.push_back(line);
				continue;
				
			}

			case PS_AUTHENTICATION:
			{

				size_t start, end;
				start = 0;
				end = line.find_first_of('=');
				std::string field = line.substr(start, end - 1);
				if(!field.compare("auth_method"))
				{
					//TODO parse auth methods nickserv, local active, local passive
				}
				else
				{
					return P_INVALID_FIELD;
				}
				continue;
			}

			case PS_RECONNECTION:
			{
				
				size_t start, end;
				start = 0;
				end = line.find_first_of('=');
				std::string field = line.substr(start, end - 1);
				std::string temp;
				if(!field.compare("auto reconnect"))
				{
					temp = line.substr(end + 2);
					ircLog::instance()->logf(FILENAME, "found reconnect; string is: %s", temp.c_str());
					if(!temp.compare("true") || !temp.compare("1"))
					{
						_autoReconnect = true;
						ircLog::instance()->logf(FILENAME, "auto reconnect = true");
					}
					else if(!temp.compare("false") || !temp.compare("0"))
					{
						_autoReconnect = false;
						ircLog::instance()->logf(FILENAME, "auto reconnect = false");
					}
					else
					{
						return  P_INVALID_VALUE;
					}

				}
				else if(!field.compare("max retries"))
				{
					_maxReconnRetries = atoi(line.substr(end+2).c_str());
					ircLog::instance()->logf(FILENAME, "max retires = %d", _maxReconnRetries);
				}
				else
				{
					return P_INVALID_FIELD;
				}
				continue;
			}

			case PS_PLUGIN_LOADING:
			{
				size_t start, end;
				start = 0;
				end = line.find_first_of('=');
				std::string field = line.substr(start, end - 1);
				std::string temp;

				if(!field.compare("mode"))
				{
					temp = line.substr(end + 2);
					if(temp.find("dynamic") != std::string::npos)
					{
						_pluginLoadMode = PLM_DYNAMIC;
					}
					else if(temp.find("static") != std::string::npos)
					{
						_pluginLoadMode = PLM_STATIC;
					}

				}
				else if(!field.compare("directory"))
				{
					_pluginDir = line.substr(end + 2);
				}

			}

			default:
				return P_INVALID_STATE;
		}
	}
	configFile.close();
	return P_SUCCESS;
}

#endif

int  OmniConfigParser::parse()
{

	ircLog::instance()->logf(FILENAME, "Parsing config file %s", OMNI_CONFIG_FILE_NAME.c_str());
	if(_reader.open(OMNI_CONFIG_FILE_NAME.c_str()))
	{
		std::cerr << FILENAME << ": error reading config file" << std::endl;
		return P_IO_ERROR;
	}

	std::vector<ConfigSection*> sections;
	//load modules
	sections.push_back(dynamic_cast<ConfigSection*>(new NickSection(_reader, this)));
	sections.push_back(dynamic_cast<ConfigSection*>(new ServerSection(_reader, this)));
	sections.push_back(dynamic_cast<ConfigSection*>(new ChannelsSection(_reader, this)));
	sections.push_back(dynamic_cast<ConfigSection*>(new PluginsSection(_reader, this)));
	sections.push_back(dynamic_cast<ConfigSection*>(new ReconnSection(_reader, this)));
	sections.push_back(dynamic_cast<ConfigSection*>(new AuthSection(_reader, this)));
	std::vector<ConfigSection*>::iterator secIter;
	for(secIter = sections.begin(); secIter != sections.end(); secIter++)
	{
		(*secIter)->clear();
	}

	std::string sect;
	int num_sections = 0;
	while(_reader.getNextSection(sect))
	{
		++num_sections;
		ircLog::instance()->logf(FILENAME, "Processing Section: %s", sect.c_str());
		std::string entry;
		ConfigSection* section;
		for(secIter = sections.begin(); secIter != sections.end(); secIter++)
		{
			//find the right module
			if((*secIter)->isSection(sect))
			{
				section = (*secIter);
				secIter = sections.end();
				secIter--;
			}
		}
		while(_reader.getNextEntry(entry))
		{
			ircLog::instance()->logf(FILENAME, "Processing Entry: %s", entry.c_str());
			section->loadEntry(entry);
		}
	}
	ircLog::instance()->logf(FILENAME, "Number of sections processed: %d", num_sections);
	return 0;
}
OmniConfigParser* OmniConfigParser::instance()
{
	if(_instance == NULL)
	{
		_instance = new OmniConfigParser();
	}

	return _instance;
}

OmniConfigParser::OmniConfigParser(){}


//NICKSECTION
OmniConfigParser::NickSection::NickSection(OmniCfgReader& rdr, OmniConfigParser* prsr):
	_reader(rdr),
	_parser(prsr)
{}

bool OmniConfigParser::NickSection::isSection(std::string section)
{
	bool retval = false;
	if(!section.compare(THIS_SECTION))
	{
		retval = true;
	}
	return retval;
}

OmniConfigParser::parseCodes OmniConfigParser::NickSection::loadEntry(std::string entry)
{
	OmniConfigParser::parseCodes retval = P_INVALID_FIELD;
	if(!entry.compare("nick"))
	{
		_parser->_nick =_reader.getString(THIS_SECTION, entry, DEFAULT_NICK);
		retval = P_SUCCESS;
	}
	return retval;
}
void OmniConfigParser::NickSection::clear()
{
	_parser->_nick = DEFAULT_NICK;
}
//SERVER SECTION
OmniConfigParser::ServerSection::ServerSection(OmniCfgReader& rdr, OmniConfigParser* prsr):
	_reader(rdr),
	_parser(prsr)
{}

bool OmniConfigParser::ServerSection::isSection(std::string section)
{
	bool retval = false;
	if(!section.compare(THIS_SECTION))
	{
		retval = true;
	}
	return retval;
}
OmniConfigParser::parseCodes OmniConfigParser::ServerSection::loadEntry(std::string entry)
{
	OmniConfigParser::parseCodes retval = P_INVALID_FIELD;
	if(!entry.compare("name"))
	{
		_parser->_serverName =_reader.getString(THIS_SECTION, entry, DEFAULT_SERVER);
		ircLog::instance()->logf(FILENAME, "Loaded Server Name: %s", _parser->_serverName.c_str());
		retval = P_SUCCESS;
	}
	else if(!entry.compare("port"))
	{
		_parser->_serverPort = _reader.getInt(THIS_SECTION, entry, DEFAULT_PORT);
		ircLog::instance()->logf(FILENAME, "Loaded Server Port: %d", _parser->_serverPort);
		retval = P_SUCCESS;
	}
	return retval;
}

void OmniConfigParser::ServerSection::clear()
{
	_parser->_serverName = DEFAULT_SERVER;
	_parser->_serverPort = DEFAULT_PORT;
}
//CHANNELS SECTION
OmniConfigParser::ChannelsSection::ChannelsSection(OmniCfgReader& rdr, OmniConfigParser* prsr):
	_reader(rdr),
	_parser(prsr)
{}

bool OmniConfigParser::ChannelsSection::isSection(std::string section)
{
	bool retval = false;
	if(!section.compare(THIS_SECTION))
	{
		retval = true;
	}
	return retval;
}
OmniConfigParser::parseCodes OmniConfigParser::ChannelsSection::loadEntry(std::string entry)
{
	OmniConfigParser::parseCodes retval = P_INVALID_FIELD;
	if(!entry.substr(0, 7).compare("channel"))
	{
		_parser->_channels.push_back(_reader.getString(THIS_SECTION, entry, "INVALID"));
		retval = P_SUCCESS;
	}
	return retval;
}
void OmniConfigParser::ChannelsSection::clear()
{
	_parser->_channels.clear();
}
//PLUGINS SECTION
OmniConfigParser::PluginsSection::PluginsSection(OmniCfgReader& rdr, OmniConfigParser* prsr):
	_reader(rdr),
	_parser(prsr)
{}

bool OmniConfigParser::PluginsSection::isSection(std::string section)
{
	bool retval = false;
	if(!section.compare(THIS_SECTION))
	{
		retval = true;
	}
	return retval;
}
OmniConfigParser::parseCodes OmniConfigParser::PluginsSection::loadEntry(std::string entry)
{
	OmniConfigParser::parseCodes retval = P_INVALID_FIELD;
	if(!entry.substr(0, 6).compare("plugin"))
	{
		_parser->_plugins.push_back(_reader.getString(THIS_SECTION, entry, "INVALID"));
		retval = P_SUCCESS;
	}
	else if(!entry.compare("mode"))
	{
		std::string temp = _reader.getString(THIS_SECTION, entry, "static" /*DEFAULT_LOAD_MODE*/);
		if(temp.find("dynamic") != std::string::npos)
		{
			_parser->_pluginLoadMode = PLM_DYNAMIC;
			retval = P_SUCCESS;
		}
		else if(temp.find("static") != std::string::npos)
		{
			_parser->_pluginLoadMode = PLM_STATIC;
			retval = P_SUCCESS;
		}

	}
	else if(!entry.compare("directory"))
	{
		_parser->_pluginDir = _reader.getString(THIS_SECTION, entry, DEFAULT_PLUGIN_DIR);
		retval = P_SUCCESS;
	}
	return retval;
}

void OmniConfigParser::PluginsSection::clear()
{
	_parser->_plugins.clear();
	_parser->_pluginLoadMode = DEFAULT_LOAD_MODE;
	_parser->_pluginDir = DEFAULT_PLUGIN_DIR;
}
//AUTH SECTION
OmniConfigParser::AuthSection::AuthSection(OmniCfgReader& rdr, OmniConfigParser* prsr):
	_reader(rdr),
	_parser(prsr)
{}

bool OmniConfigParser::AuthSection::isSection(std::string section)
{
	bool retval = false;
	if(!section.compare(THIS_SECTION))
	{
		retval = true;
	}
	return retval;
}
OmniConfigParser::parseCodes OmniConfigParser::AuthSection::loadEntry(std::string entry)
{
}

void OmniConfigParser::AuthSection::clear()
{
}
//RECONN SECTION
OmniConfigParser::ReconnSection::ReconnSection(OmniCfgReader& rdr, OmniConfigParser* prsr):
	_reader(rdr),
	_parser(prsr)
{}

bool OmniConfigParser::ReconnSection::isSection(std::string section)
{
	bool retval = false;
	if(!section.compare(THIS_SECTION))
	{
		retval = true;
	}
	return retval;
}
OmniConfigParser::parseCodes OmniConfigParser::ReconnSection::loadEntry(std::string entry)
{
	OmniConfigParser::parseCodes retval = P_INVALID_FIELD;
	if(!entry.compare("auto reconnect"))
	{
		_parser->_autoReconnect = _reader.getBool(THIS_SECTION, entry, DEFAULT_AUTOCONNECT);
		retval = P_SUCCESS;

	}
	else if(!entry.compare("max retries"))
	{
		_parser->_maxReconnRetries = _reader.getInt(THIS_SECTION, entry, DEFAULT_RETRIES);
		retval = P_SUCCESS;
	}
	return retval;
}
void OmniConfigParser::ReconnSection::clear()
{
	_parser->_autoReconnect = DEFAULT_AUTOCONNECT;
	_parser->_maxReconnRetries = DEFAULT_RETRIES;
}
