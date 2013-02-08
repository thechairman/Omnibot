
#ifndef _OMNI_DROP_CMD_H_
#define _OMNI_DROP_CMD_H_

#include "OmniCommand.h"


class OmniDropCmd: public OmniCommand

{
	public:
//		void setTools(ircInterface*, PluginManager*);
//		void setPrefix(char);
		std::string name();
		std::string use();
		std::string help();
//		std::string resultStr();
//		void clear();
		bool exec(ircMessage& );
//		bool isCommandString(ircMessage& );
	private:
		static const std::string NAME;
		static const std::string USE;
		static const std::string HELP;

/*		std::string _prefix;
		std::string _errorString;
		std::string _results;
		ircInterface* _irc;
		PluginManager* _plugins;
*/
};

#endif
