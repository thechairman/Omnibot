#ifndef _OMNI_PLUGIN_
#define _OMNI_PLUGIN_

#include "ircNotifyClasses.h"
#include "PluginUtils.h"


class OmniPlugin {
	public:
		static void onMessage(OmniPlugin*, ircMessage);
		static void startThread(void*);
		virtual void handleMessage(ircMessage*) = 0;
		virtual bool init(PluginUtils*) = 0;
		virtual void wrapUp() = 0;
		virtual std::string name() = 0;


};

#endif
