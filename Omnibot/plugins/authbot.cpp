#include "authbot.h"

void hURLbot::onMessage(ircMessage& msg){
	if(!msg.message().compare("!userList")){
		std::cout << "hURLbot: gonna hURL" << std::endl;
		ircUser* user = msg.user();

		utils->sendPM(user->nick(), "Here ya go:");

		std::vector<ircUser*>::iterator iter;
		std::vector<ircUser*> users = utils->getAllUsers();

		for(iter = users.begin(); iter != users.end(); iter++){
			utils->sendPM(user->nick(), (*iter));
		}

	}

}
