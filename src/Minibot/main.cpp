#include"ircInterface.h"
#include"ircInterfaceClient.h"

#include <iostream>

#include<poll.h>
#include<unistd.h>

class testClass: public ircInterfaceClient{
	public:
		void alertEvent(ircEvent);
		void alertMessage(ircMessage);
};

void testClass::alertEvent(ircEvent e){}
void testClass::alertMessage(ircMessage m){
	std::cout << "TestClass: " << m.user()->nick() << ": " << m.message() << std::endl;
}

int main()
{
	ircInterface irc;
	//drop the client in here
	testClass tester;
	irc.registerForNotify(&tester);
	
	irc.connect("madhax.net", 6667);
//	sleep(2);
	irc.registerUser("Omnibot", "Omnibot", "Omnibot");
//	sleep(10);
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
