#ifndef _OMNI_PLUGIN_
#define _OMNI_PLUGIN_

#include "ircNotifyClasses.h"
#include "PluginUtils.h"


class OmniPluginInterface {
	public:
		static void onMessage(void*);
		virtual void handleMessage(ircMessage*) = 0;
		virtual bool init(PluginUtils*) = 0;
		virtual void wrapUp() = 0;


};

#endif
