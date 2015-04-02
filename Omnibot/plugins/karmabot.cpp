#include "karmabot.h"
#include <fstream>
#include <sstream>

const std::string karmabot::KARMA_FNAME = "karma.txt";
const std::string karmabot::INCREMENTOR = "++";
const std::string karmabot::DECREMENTOR = "--";
const std::string karmabot::COMMAND_STR = "!karma";

void karmabot::onMessage(ircMessage& msg)
{
//	if(!msg.compare("!karma"))
//	{
//		if(!msg.user().isAuthenticated())
//		{
//			utils->sendMessage(msg.channel(), msg.user().nick() + ": You need to authenticate with NickServe");
//			return;
//		}
//		unsigned int karma = _karma[msg.user().userId()];
//
//		std::stringstream str << msg.user()->nick() << ": " << karma;
//		utils->sendMessage(msg.channel(), str.str());
//	}


	size_t increment_loc = msg.message().find(INCREMENTOR);
	size_t decrement_loc = msg.message().find(DECREMENTOR);
	bool updated = false;
	//if we are trying to get somebody's karma total
	if(!msg.message().substr(0, COMMAND_LEN).compare(COMMAND_STR))
	{
		int karma = 0;
		std::string target;

		if(msg.message().find(" ") == std::string::npos)
		{
			target = msg.user().nick();
		}
		else
		{
			//get first character past the space
			int start = msg.message().find(" ") + 1;
			int dist = msg.message().find(" ", start) - start;
			target = msg.message().substr(start, dist);
		}

		std::map<std::string, int>::iterator nick_pos;
		nick_pos = _karma.find(target);

		if(nick_pos != _karma.end())
		{
			karma = nick_pos->second;
		}
		else
		{
			karma = 0;
			std::pair<std::string, int> thing(target, karma);
			_karma.insert(thing);
			updated = true;
		}
		std::stringstream strstr;
		strstr << target << " karma: " << karma;
		utils->sendMessage(msg.channel(), strstr.str());
		
	}
	else if (increment_loc != std::string::npos)
	{
		std::string nick;
	       	nick = msg.message().substr( 0, increment_loc);
		
		stripString(nick, ": ");
	        stripString(nick, ":");	

		if(nick.find(" ") == std::string::npos)
		{
			if(nick.compare(msg.user().nick()))
			{
				incrementKarma(nick);
				updated = true;
			}
			else
			{
				utils->sendMessage(msg.channel(), msg.user().nick() + ": You can't karma yourself!");
			}
		}

	}
	else if (decrement_loc != std::string::npos)
	{
		std::string nick;
	       	nick = msg.message().substr( 0, decrement_loc);

		stripString(nick, ": ");
	        stripString(nick, ":");	

		if(nick.find(" ") == std::string::npos)
		{
			if(nick.compare(msg.user().nick()))
			{	
				decrementKarma(nick);
				updated = true;
			}
			else
			{
				utils->sendMessage(msg.channel(), msg.user().nick() + ": I just stopped you from doing something dumb");
			
			}
		}
	}

//	if(msg.message().find(" ") == std::string::npos)
//	{
//		ircUser* target == NULL;
//		if(msg.message().find("++") != std::string::npos)
//		{
//			target = utils->getUser(0, msg.message().find("++"));
//
//			incrementKarma(target);
//
//		}
//		else if (msg.message().find("--") != std::string::npos)
//		{
//			target = utils->getUser(0, msg.message().find("--"));
//			decrementKarma(target);
//		}
//		else
//		{
//			return;
//		}
//	}

	if(updated)
	{
		saveKarma();
	}
}

void karmabot::onOmniCommConnect(OmniCommChannel* o){}
void karmabot::decrementKarma(std::string nick)
{
	std::map<std::string, int>::iterator iter;
	iter = _karma.find(nick);
	if(iter == _karma.end())
	{
		_karma[nick] = -1;
	}
	else
	{
		 (_karma[nick])--; 
	}
}

void karmabot::incrementKarma(std::string nick)
{

	std::map<std::string, int>::iterator iter;
	iter = _karma.find(nick);
	if(iter == _karma.end())
	{
		_karma[nick] = 1;
	}
	else
	{
		 (_karma[nick])++; 
	}
}

void karmabot::loadKarma()
{
	std::ifstream karmaFile;
	karmaFile.open(KARMA_FNAME.c_str());

	if(karmaFile.fail())
	{
		//TODO write something to std::err
	}
	else
	{
		std::string fileLine;
		while(getline(karmaFile, fileLine))
		{
			std::string nick;
			int karma;
			std::stringstream strstr(fileLine);
			strstr >> nick;
			strstr >> karma;
			_karma[nick] = karma;
		}
	}
	karmaFile.close();

}

void karmabot::saveKarma()
{
	std::ofstream karmaFile;
	karmaFile.open(KARMA_FNAME.c_str(), std::ios_base::out | std::ios_base::trunc);

	std::map<std::string, int>::iterator iter;

	for(iter = _karma.begin(); iter != _karma.end(); ++iter)
	{
		karmaFile << iter->first << " " << iter->second << std::endl;
	}
	karmaFile.close();

}

void karmabot::stripString(std::string& theString,  const std::string& substr)
{
	int i = theString.find(substr);
	while(i != std::string::npos)
	{
		theString.erase(i, substr.length());
		i = theString.find(substr);
	}
}

bool karmabot::init(PluginUtils* utils_)
{
	utils = utils_;

	loadKarma();

	return true;

}

void karmabot::wrapUp()
{
	saveKarma();
}

std::string karmabot::name()
{
	std::string name = "karmabot";
	return name;
}
