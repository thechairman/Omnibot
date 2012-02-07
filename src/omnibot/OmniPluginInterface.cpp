
#include "OmniPluginInterface.h"

void OmniPlugin::passMessage(OmniPlugin* plugin, ircMessage& msg){

	std::cout << "omniPlugin: passMessage: Message size: " << msg.message().size() << std::endl;
	std::pair<OmniPlugin*, ircMessage> *apair;

       	apair = new std::pair<OmniPlugin*, ircMessage>(plugin, msg);

	if(plugin->thread.isRunning()){
		std::cout << "omniplugin: thread running adding messge to the queue" << std::endl;
		plugin->thread.addTask(&startThread, (void*)apair);
		
	}else{
		std::cout << "omniplugin: starting thread" <<std::endl;
		plugin->thread.callBack(&startThread);
		plugin->thread.arg((void*) apair);
		plugin->thread.start();
	}

}

void* OmniPlugin::startThread(void* pair_){

	std::cout << "OmniPlugin: running startThread" << std::endl;

	std::pair<OmniPlugin*, ircMessage>* pair = (std::pair<OmniPlugin*, ircMessage>*) pair_;


	std::cout << "omniPlugin: startThread: Message size: " << pair->second.message().size() << std::endl;
	pair->first->onMessage(pair->second);
}
