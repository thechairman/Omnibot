#ifndef _OMNI_PLUGIN_
#define _OMNI_PLUGIN_

#include "Threads/pthread_Thread.h"

#include "Threads/OmniThread.h"
#include "Threads/OmniThreadedClass.h"
#include <utility>
#include <deque>
#include "ircNotifyClasses.h"
#include "PluginUtils.h"

#include "OmniCommChannel.h"

class PluginUtils;

//class OmniPluginThread: public OmniThread{
//	public:
//		//fucntion takes both plugin and pair
//		
//		//overide start to pass this function instead
//};


class OmniPlugin {
	public:

		OmniPlugin();

		static void passMessage(OmniPlugin*, ircMessage&);
		static void passOmniCommChannel(OmniPlugin*, OmniCommChannel*);
		//static void* startThread(void*);
		virtual void onMessage(ircMessage&) = 0;
		virtual void onOmniCommConnect(OmniCommChannel*);
		virtual bool init(PluginUtils*) = 0;
		virtual void wrapUp() = 0;
		virtual std::string name() = 0;

	private:
		class PluginRunner: public OmniThreadedClass
		{
			public:
				PluginRunner();
				void runA();
				void runB();
				void runC();
				void runD();
			private:
				OmniPlugin* plugin;
				ircMessage msg;
				OmniCommChannel* channel;
				
			friend class OmniPlugin;
		};


		pthread_Thread thread;


};

#endif
