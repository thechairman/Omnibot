#include<sstream>
#include "ircNickListParser.h"
#include "ircEvents.h"

void ircNickListParser::setNickPrefixes(std::string prefixes)
{
	_prefixes = prefixes;
};

std::vector<ircEvent> ircNickListParser::parseNickList(std::string nicklist, std::string channel)
{
	std::stringstream nicks(nicklist);
	std::string currentNick;
	std::vector<ircEvent> nicksInList;

	while(getline(nicks, currentNick, ' '))
	{
		if(_prefixes.find(currentNick[0]) != std::string::npos)
			currentNick = currentNick.substr(1);

		nicksInList.push_back(ircEvent_join(channel, currentNick));
	}

	return nicksInList;
}
