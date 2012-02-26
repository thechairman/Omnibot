#include "NickManager.h"

void NickManager::channelnicks(std::string channel_ std::vector<std::string> vect){
	std::vector<std::string>::iterator itr;
	for(itr = vect.begin(), itr != vect.end(), vect++){

		//check to see if its allready in users
		
		//if not create a new user
		
	}
}

void NickManager::changeNick(std::string oldNick, std::string newNick){
	//find nick  in users
	
	//change nick value in user
}

void NickManager::joinChannel(std::string channel, std::string nick){
	//find out if nick exists
	//
	//if not, create one;
	//
	//put it in the appropritate chanel
}

void NickManager::leaveChannel(std::string channel, std::string nick){
//get nick from vector and delete it

}

void NickManager::quit(std::string nick){
	//get nick from vector and delete it
}

void verifyUserStatus(ircUser* user_){

}

std::vector<ircUser*>::iterator NickManager::getUserByNick(std::string nick){
	std::vector<ircUser*>::iterator iter;
	for(iter = users.begin(); iter!= users.end(); iter++){
		if(!nick.compare((*iter)->nick())){
			break;
		}
	}

	return iter;
}
