#include "karmabot.h"

void karmabot::onMessage(ircMessage& msg)
{
	if(!msg.compare("!karma"))
	{
		if(!msg.user().isAuthenticated())
		{
			utils->sendMessage(msg.channel(), msg.user().nick() + ": You need to authenticate with NickServe");
			return;
		}
		unsigned int karma = _karma[msg.user().userId()];

		std::stringstream str << msg.user()->nick() << ": " << karma;
		utils->sendMessage(msg.channel(), str.str());
	}

	if(msg.message().find(" ") == std::string::npos)
	{
		ircUser* target == NULL;
		if(msg.message().find("++") != std::string::npos)
		{
			target = utils->getUser(0, msg.message().find("++"));

			incrementKarma(target);

		}
		else if (msg.message().find("--") != std::string::npos)
		{
			target = utils->getUser(0, msg.message().find("--"));
			decrementKarma(target);
		}
		else
		{
			return;
		}
	}

}

void karmabot::decrementKarma(ircUser* target)
{
	
}

void karmabot::incrementKarma(ircUser* target)
{

}

void karmabot::loadKarma()
{
}

void karmabot::saveKarma()
{

}

bool karmabot::init(PluginUtils* utils_)
{
	utils = utils_;

	loadKarma();

	return true;

}

void karmabot::wrapup()
{
	saveKarma();
}

std::string karmabot::name()
{
	std::string name = "karmabot";
	return name;
}
