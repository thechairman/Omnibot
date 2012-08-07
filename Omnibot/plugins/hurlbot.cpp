#include "hurlbot.h"

void hURLbot::onMessage(ircMessage& msg){
	std::cout<<"hURLbot: got a message of length: " << msg.message().size() << std::endl;
	if(!msg.message().compare("!hurl")){
		std::cout << "hURLbot: gonna hURL" << std::endl;
		ircUser user = msg.user();

		//if(user == NULL)
		//	std::cout << "this fraking thing is null too" << std::endl;

		utils->sendPM(user.nick(), "Here ya go:");

		std::deque<std::string>::iterator iter;
		for(iter = urls.begin(); iter != urls.end(); iter++){
			utils->sendPM(user.nick(), (*iter));
		}

	}
	else {
		std::cout<< "hurlbot: gotta string, wasn't hurl... it was : "<<msg.message() << std::endl;
		size_t pos = msg.message().find("http");
		if ( pos != std::string::npos){
			std::string url = msg.message().substr(pos, msg.message().find_first_of(' ', pos));
			if(urls.size() == MAX_URLS){
				urls.pop_front();
			}
			urls.push_back(url);
		}
		
	}

}

void hURLbot::onOmniCommConnect(OmniCommChannel* channel){}

bool hURLbot::init(PluginUtils* utils_){
	utils = utils_;
	return true;
}

void hURLbot::wrapUp(){

}

std::string hURLbot::name(){
	std::string name = "hurlbot";
	return name;
}
