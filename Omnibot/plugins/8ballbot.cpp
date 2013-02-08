

#include "8ballbot.h"

const std::string eightBallBot::replys[NUM_OF_STATEMENTS] ={

	"Signs point to yes.",
	"Yes.",
	"Reply hazy, try again.",
	"Without a doubt",
	"Sources say no.",
	"As I see it, yes",
	"Rely on it.",
	"Ask later",
	"Outlook isn't Good",
	"Decidedly So.",
	"Better not to say now.",
	"Doubtful",
	"Definitely",
	"It's certain",
	"Can't see Now",
	"Most Likely",
	"Try agin",
	"I say no.",
	"Outlook looks good",
	"Don't count on it"
};
//const std::string eightBallBot::replys[0] = "Signs point to yes.";
//const std::string eightBallBot::replys[1] = "Yes.";
//const std::string eightBallBot::replys[2] = "Reply hazy, try again.";
//const std::string eightBallBot::replys[3] = "Without a doubt";
//const std::string eightBallBot::replys[4] = "Sources say no.";
//const std::string eightBallBot::replys[5] = "As I see it, yes";
//const std::string eightBallBot::replys[6] = "Rely on it.";
//const std::string eightBallBot::replys[7] = "Ask later";
//const std::string eightBallBot::replys[8] = "Outlook isn't Good";
//const std::string eightBallBot::replys[9] = "Decidedly So.";
//const std::string eightBallBot::replys[10] = "Better not to say now.";
//const std::string eightBallBot::replys[11] = "Doubtful";
//const std::string eightBallBot::replys[12] = "Definitely";
//const std::string eightBallBot::replys[13] = "It's certain";
//const std::string eightBallBot::replys[14] = "Can't see Now";
//const std::string eightBallBot::replys[15] = "Most Likely";
//const std::string eightBallBot::replys[16] = "Try agin";
//const std::string eightBallBot::replys[17] = "I say no.";
//const std::string eightBallBot::replys[18] = "Outlook looks good";
//const std::string eightBallBot::replys[19] = "Don't count on it";

void eightBallBot::onMessage(ircMessage& msg)
{
	std::string cmd = msg.message().substr(0, 6);
	if(!cmd.compare("!8ball"))
	{
		_utils->sendMessage(msg.channel(), replys[getRandom()]);
	}
}
void eightBallBot::onOmniCommConnect(OmniCommChannel* occ){}
bool eightBallBot::init(PluginUtils* pus)
{
	_utils = pus;
}
void eightBallBot::wrapUp(){}
std::string eightBallBot::name() 
{
	std::string name = "8ballbot";
	return name;
};
//void eightBallBot::sendReply()
//{
//	_utils->sendMesage(
//}

int eightBallBot::getRandom(){
	srand(time(NULL));

	return rand() % NUM_OF_STATEMENTS;
}
