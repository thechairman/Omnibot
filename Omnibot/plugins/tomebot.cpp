#include <cstring>
#include <ctime>
#include <fstream>
#include "tomebot.h"

const char tomebot::tomeFile[30] = "tome.txt";

void tomebot::onMessage(ircMessage& msg){
	std::cout<<"tomebot: got a message of length: " << msg.message().size() << std::endl;
	if(!msg.message().compare("!tome")){
		int lines;
		lines = atoi(msg.message().substr(10).c_str());
	

		//write lines to file		
		writeLines(lines);
	}
	else {
		std::cout<< "hurlbot: gotta string, wasn't hurl... it was : "<<msg.message() << std::endl;
		std::string line = getLineTimeStamp() + " <" + msg.user().nick() +"> " +msg.message();
		if(lines.size() == MAX_LINES){
			lines.pop_front();
		}
		lines.push_back(line);
		
	}

}

void tomebot::onOmniCommConnect(OmniCommChannel* channel){}

bool tomebot::init(PluginUtils* utils_){
	utils = utils_;
	return true;
}

void tomebot::wrapUp(){}

void tomebot::writeLines(int numOfLines){

	//openfile
	std::ofstream tome;
	tome.open(tomeFile, std::ios_base::app);

	tome << std::endl << getEntryTimeStamp() << std::endl;
	
	//count back from the end to the start line
	int startIndex = MAX_LINES - numOfLines;
	
	//write those lines to file
	for(unsigned int i = startIndex; i < MAX_LINES; ++i)
	{
		tome << lines.at(i) << std::endl;
	}
	
	//close file
	tome.close();

}

std::string tomebot::name(){
	std::string name = "tomebot";
	return name;
}

std::string tomebot::getLineTimeStamp()
{
	std::string date;
	char cdate[TIME_BUFF_SIZE];

	time_t rawtime;
	struct tm* timeInfo;

	time(&rawtime);
	timeInfo = localtime(&rawtime);

	memset(cdate, '\0', TIME_BUFF_SIZE);
	strftime(cdate, TIME_BUFF_SIZE, "%m-%d-%Y - %X", timeInfo);
	date = cdate;

	return date;
}
std::string tomebot::getEntryTimeStamp()
{
	std::string date;
	char cdate[TIME_BUFF_SIZE];

	time_t rawtime;
	struct tm* timeInfo;

	time(&rawtime);
	timeInfo = localtime(&rawtime);

	memset(cdate, '\0', TIME_BUFF_SIZE);
	strftime(cdate, TIME_BUFF_SIZE, "%c", timeInfo);
	date = cdate;

	return date;
}
