#include "ircInterface.h"
#include "omnibot.h"

#include "OmniConfigParser.h"

#include<poll.h>
#include<unistd.h>

#include<iostream>

void connectWithConfig(ircInterface& irc)
{
	OmniConfigParser* parser = OmniConfigParser::instance();
	parser->parse();

	std::cout << "serverName = " << parser->serverName() << std::endl;
	std::cout << "serverPort = " << parser->serverPort() << std::endl;
	std::cout << "nick = " << parser->nick() << std::endl;

	irc.connect(parser->serverName(), parser->serverPort());
	irc.registerUser(parser->nick(), parser->nick(), parser->nick());

	std::vector<std::string> channels = parser->channels();

	for(size_t i = 0; i < channels.size(); ++i)
	{
		irc.join(channels[i]);
	}

}

int main(){	
	std::cout << "initilizing the irc interface..." << std::endl;
	//ircInterface irc;
	//std::cout << std::hex << &irc << std::dec << std::endl;
	std::cout << "firing up the omnibot..." <<std::endl;
	omnibot omni;
	//irc.registerForNotify(&omni);

	std::cout << "connecting to server..." << std::endl;
/*	irc.connect("madhax.net", 6667);
	irc.registerUser("Omnibot", "Omnibot", "Omnibot");
	irc.join("#alpha1");
//	irc.join("#main");
	irc.join("#test2");
*/

	//connectWithConfig(irc);
	omni.connect();


	bool loop = true;
	pollfd cinfd[1];
	cinfd[0].fd = 0;
	cinfd[0].events = POLLIN;
	char pain[3];
	while(loop){

		if(poll(cinfd, 1, 1000)){
			std::cin >> pain;
		}

		if(pain[0] == 'q' || pain[0] == 'Q'){
		//	loop = false;	
		}
	}
	std::cout << "exit while loop in main" << std::endl;
	//irc.quit();
}

