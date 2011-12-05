#include "ircInterface.h"
#include "omnibot.h"

#include<poll.h>
#include<unistd.h>
int main(){
	ircInterface irc;
	omnibot omni(irc);

	irc.connect("madhax.net", 6667);
	irc.registerUser("Omnibot", "Omnibot", "Omnibot");
	irc.join("#alpha1");
	irc.join("#main");


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
			loop = false;	
		}
	}

	irc.quit();
}


