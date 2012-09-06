
#include "OmniPluginInterface.h"
#include "ircLog.h"

const std::string FILENAME = "OmniPlugin.cpp";

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
	//std::cout << "adding task to thread" << std::endl;
	//std::cout << "createing PluginRunner" << std::endl;
	
	ircLog::instance()->logf(FILENAME, "adding task to thread");
	ircLog::instance()->logf(FILENAME, "creating plugin runner");

	OmniPlugin::PluginRunner* runner = new PluginRunner();

	//std::cout << "assigning PluginRunner internals" << std::endl;
	ircLog::instance()->logf(FILENAME, "assigning PluginRunner internals");
	runner->plugin = plugin;

	runner->msg = msg;

	//setup the the thread and fire it off
	//std::cout << "queue task" <<std::endl;
	ircLog::instance()->logf(FILENAME, "Queueing task");
	plugin->thread.addTask(runner, OmniThreadedClass::MODE_A);

}

void OmniPlugin::passOmniCommChannel(OmniPlugin* plugin, OmniCommChannel* chan)
{
	OmniPlugin::PluginRunner* runner = new PluginRunner();

	runner->plugin = plugin;

	runner->channel = chan;

	plugin->thread.addTask(runner, OmniThreadedClass::MODE_B);

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

void OmniPlugin::PluginRunner::runB(){
	plugin->onOmniCommConnect(channel);
	delete this;
}
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
