
#include "OmniPluginInterface.h"

void OmniPlugin::passMessage(OmniPlugin* plugin, ircMessage* msg){
	std::pair<OmniPlugin*, ircMessage*> *apair;
       	apair = new std::pair<OmniPlugin*, ircMessage*>(plugin, msg);

	if(plugin->thread.isRunning()){
		plugin->thread.addTask(&startThread, (void*)apair);
		
	}else{
		plugin->thread.callBack(&startThread);
		plugin->thread.arg((void*) apair);
		plugin->thread.start();
	}

}

void* OmniPlugin::startThread(void* pair_){
	std::pair<OmniPlugin*, ircMessage>* pair = (std::pair<OmniPlugin*, ircMessage>*) pair_;
	pair->first->onMessage(pair->second);
}
