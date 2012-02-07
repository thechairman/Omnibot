#ifndef _NICK_MANAGER_
#define _NICK_MANAGER_

#include"ircNotifyClasses.h"
#include"ircInterface.h"

#include<string>
#include<vector>
#include<map>

class NickManager{
	

	private:
		static unsigned int nextId;
		static unsigned int maxRegId;

		std::vector<ircUser*> users;
		std::map<std::string, std::vector<ircUser*> > channels;
		std::map<std::string, unsigned int> registeredUserIds;

		void channelNicks(std::string, std::vector<std::string>);
		void changeNick(std::string, std::string);
		void joinChannel(std::string, std::string);
		void leaveChannel(std::string, std::string);
		void quit(std::string);
		void verifyUserStatus(ircUser*);

		std::vector<ircUser*>::iterator getUserByNick(std::string);

		void writeOutData();
		void readInData();

		friend class ircInterface;
};

#endif
