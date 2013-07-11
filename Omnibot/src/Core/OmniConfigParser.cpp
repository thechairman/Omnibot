#include <fstream>
#include <algorithm>
#include <iostream>

#include "OmniConfigParser.h"

#include "ircLog.h"

const std::string FILENAME = "OmniConfigParser.cpp";

const std::string OmniConfigParser::OMNI_CONFIG_FILE_NAME = "omniserverdata.cfg";

const std::string OmniConfigParser::NICKS = "nick";
const std::string OmniConfigParser::SERVER = "server";
const std::string OmniConfigParser::CHANNELS = "channels";
const std::string OmniConfigParser::PLUGINS = "plugins";
const std::string OmniConfigParser::AUTHENTICATION = "authentication";
const std::string OmniConfigParser::RECONNECTIONS = "reconnections";
const std::string OmniConfigParser::PLUGIN_LOADING = "plugin loading";

const std::string 	OmniConfigParser::DEFAULT_NICK = "Omnibot";
const std::string 	OmniConfigParser::DEFAULT_SERVER = "";
const int 		OmniConfigParser::DEFAULT_PORT = 6667;
const bool 		OmniConfigParser::DEFAUT_AUTOCONNECT = false;
const int  		OmniConfigParser::DEFAULT_RETRIES = 0;
//const std::string 	OmniConfigParser::DEFAULT_PLUGINDIR = ;
//const pluginLoadModes 	OmniConfigParser::DEFAULT_LOADMODE = ;


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

OmniConfigParser* OmniConfigParser::instance()
{
	if(_instance == NULL)
	{
		_instance = new OmniConfigParser();
	}

	return _instance;
}

OmniConfigParser::OmniConfigParser(){}
