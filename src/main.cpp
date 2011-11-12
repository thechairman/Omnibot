#include"ircInterface.h"
#include"ircInterfaceClient.h"


#include<poll.h>

int main()
{
	ircInterface irc;
	//drop the client in here
	
	irc.connect("madhax.net" 6667);
	irc.registerNick("minibot", "minibot", "minibot");
	irc.join("#alpha1");
	
	bool loop = true;
	pollfd cinfd[1];
	cinfd[0].fd = 0;
	cinfd[0].events = POLLIN;
	char pain[3];
	while(loop){

		if(poll(cinfd, 1, 1000)){
			cin >> pain;
		}

		if(pain[0] == 'q' || pain[0] == 'Q'){
			loop = false;	
		}
	}

	irc.quit();
	
		
}
