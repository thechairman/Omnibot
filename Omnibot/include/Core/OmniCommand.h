#ifndef _OMNI_COMMAND_H_
#define _OMNI_COMMAND_H_
#include <vector>
#include <string>

#include "ircNotifyClasses.h"
#include "ircInterface.h"

#include "PluginManager.h"

class OmniCommand
{
	public:
		virtual void setTools(ircInterface*, PluginManager*);
		virtual void setPrefix(char);
		virtual std::string name() =0;
		virtual std::string use() =0;
		virtual std::string help() =0;
		virtual std::string resultStr();
		virtual void clear();
		virtual bool exec(ircMessage& ) =0;
		virtual bool isCommandString(ircMessage& );
	
		//static void getCommands(std::vector&);
		//
	protected:
		ircInterface* _irc;
		PluginManager* _plugins;
		std::string _prefix;
		std::string _results;

};

#endif

