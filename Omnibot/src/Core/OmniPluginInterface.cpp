
#include "OmniPluginInterface.h"

void OmniPlugin::passMessage(OmniPlugin* plugin, ircMessage& msg){

//	std::cout << "omniPlugin: passMessage: Message size: " << msg.message().size() << std::endl;
//	std::pair<OmniPlugin*, ircMessage> *apair;
//
//       	apair = new std::pair<OmniPlugin*, ircMessage>(plugin, msg);
//
//	if(plugin->thread.isRunning()){
//		std::cout << "omniplugin: thread running adding messge to the queue" << std::endl;
//		plugin->thread.addTask(&startThread, (void*)apair);
//		
//	}else{
//		std::cout << "omniplugin: starting thread" <<std::endl;
//		plugin->thread.callBack(&startThread);
//		plugin->thread.arg((void*) apair);
//		plugin->thread.start();
//	}
	std::cout << "adding task to thread" << std::endl;
	std::cout << "createing PluginRunner" << std::endl;	
	OmniPlugin::PluginRunner* runner = new PluginRunner();

	std::cout << "assigning PluginRunner internals" << std::endl;
	runner->plugin = plugin;

	runner->msg = msg;

	//setup the the thread and fire it off
	std::cout << "queue task" <<std::endl;
	plugin->thread.addTask(runner, OmniThreadedClass::MODE_A);

}

OmniPlugin::OmniPlugin(){
	thread.init();
}




OmniPlugin::PluginRunner::PluginRunner():plugin(NULL){}

void OmniPlugin::PluginRunner::runA()
{
	plugin->onMessage(msg);
	//not sure this a a good Idea w but it works for now
	delete this;
}

void OmniPlugin::PluginRunner::runB(){}
void OmniPlugin::PluginRunner::runC(){}
void OmniPlugin::PluginRunner::runD(){}


//void* OmniPlugin::startThread(void* pair_){
//
//	std::cout << "OmniPlugin: running startThread" << std::endl;
//
//	std::pair<OmniPlugin*, ircMessage>* pair = (std::pair<OmniPlugin*, ircMessage>*) pair_;
//
//
//	std::cout << "omniPlugin: startThread: Message size: " << pair->second.message().size() << std::endl;
//	pair->first->onMessage(pair->second);
//}
