#include <string>
#include <vector>

#include "ircNotifyClasses.h"


class ircNickListParser{

	public:
	void setNickPrefixes(std::string);
	std::vector<ircEvent> parseNickList(std::string, std::string);

	private:
	std::string _prefixes;
};

