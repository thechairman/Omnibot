#ifndef _OMNI_PLUGIN_
#define _OMNI_PLUGIN_

#include "OmniThread.h"
#include "ircNotifyClasses.h"
#include "PluginUtils.h"
#include <utility>

class PluginUtils;

class OmniPluginThread: public OmniThread{
	public:
		//fucntion takes both plugin and pair
		
		//overide start to pass this function instead
};


class OmniPlugin {
	public:
		static void passMessage(OmniPlugin*, ircMessage*);
		static void* startThread(void*);
		virtual void onMessage(ircMessage&) = 0;
		virtual bool init(PluginUtils*) = 0;
		virtual void wrapUp() = 0;
		virtual std::string name() = 0;

	private:
		OmniThread thread;


};

#endif
