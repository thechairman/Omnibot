#include "hurlbot.h"

void hURLbot::onMessage(ircMessage& msg){
	std::cout<<"hURLbot: got a message of length: " << msg.message().size() << std::endl;
	if(!msg.message().compare("!hurl")){
		std::cout << "hURLbot: gonna hURL" << std::endl;
		ircUser user = msg.user();

		//if(user == NULL)
		//	std::cout << "this fraking thing is null too" << std::endl;

		utils->sendPM(user.nick(), "Here ya go:");

		std::deque<stampedUrl>::iterator iter;
		for(iter = urls.begin(); iter != urls.end(); iter++){
			utils->sendPM(user.nick(), (*iter).url);
		}

	}
	else {
		std::cout<< "hurlbot: gotta string, wasn't hurl... it was : "<<msg.message() << std::endl;
		size_t pos = msg.message().find("http");
		if ( pos != std::string::npos){
			std::string url = msg.message().substr(pos, msg.message().find_first_of(' ', pos));
			stampedUrl newUrl;
			time(&newUrl.timeStamp);
			newUrl.url = url;
			if(difftime(newUrl.timeStamp, lastCulling) > CULL_INTERVAL)
			{
				cullStaleUrls();
				time(&lastCulling);
			}
			
			if(urls.size() == MAX_URLS){
				urls.pop_front();
			}

			urls.push_back(newUrl);
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
void hURLbot::cullStaleUrls(){
	size_t initialSize = urls.size();
	for(unsigned int i = 0; i < initialSize; ++i)
	{
		stampedUrl url = urls.front();
		time_t currentTime;
		time(&currentTime);

		if(difftime(currentTime, url.timeStamp) < TWENTY_FOUR_HOURS)
		{
			break;
		}

		urls.pop_front();
	}	
}
