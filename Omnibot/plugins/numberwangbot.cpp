#include <iostream>
#include <string>
#include <time.h>

#include "numberwangbot.h"

#ifdef DYNAMIC_LOADING
extern "C" OmniPlugin* CreatePlugin()
{
	return static_cast<OmniPlugin*> (new numberwangBot());
}
#endif

void numberwangBot::onMessage(ircMessage& msg)
{
	static int mask = 0x07;
	static time_t wangernumbStart;
	if(msg.message().find_first_of("0123456789") != 18446744073709551615)
	{
		int seed = rand();
		if(!mask && difftime(time(0x0),wangernumbStart) > 90)
			mask=0x07;
		if (!(seed&mask))
		{
			switch((seed >> 4) & 0x0F) // Remember as in Texas Hold-em you need to burn a bit
			{
				case 1:
				case 2:
				case 3:
				case 5:
				case 8:
					_utils->sendMessage(msg.channel(), "Däs Ist Nümberwang!");
					break;
				case 0xF:
					_utils->sendMessage(msg.channel(), "LET'S PLAY WANGERNUMB!");
					mask=0;
					wangernumbStart = time(0x0);
					break;
				default:
					 _utils->sendMessage(msg.channel(), "That's Numberwang!");
			}
		}
	}
}

void numberwangBot::onOmniCommConnect(OmniCommChannel* occ){}
bool numberwangBot::init(PluginUtils* pus)
{
	_utils = pus;
}
void numberwangBot::wrapUp(){}
std::string numberwangBot::name()
{
	std::string name = "NumberwangBot";
	return name;
};
