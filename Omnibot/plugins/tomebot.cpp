#include <cstring>
#include <ctime>
#include <fstream>

#include "ircLog.h"
#include "tomebot.h"

#define FILENAME "tomebot.cpp"

#ifdef DYNAMIC_LOADING
extern "C" OmniPlugin* CreatePlugin()
{
	return static_cast<OmniPlugin*> (new tomebot());
}
#endif

const char tomebot::tomeFile[30] = "tome.txt";

void tomebot::onMessage(ircMessage& msg){
//	std::cout<<"tomebot: got a message of length: " << msg.message().size() << std::endl;	
	ircLog::instance()->logf(FILENAME, "got a message %d chars long, %s", msg.message().size(), msg.message().c_str());
	if(!msg.message().substr(0, CMD_LEN).compare("!tome")  && msg.message().size() > CMD_LEN +  1) //add 1 for the sapce
	{

		int lines;
		lines = atoi(msg.message().substr(CMD_LEN).c_str());
	

		//write lines to file		
		ircLog::instance()->logf(FILENAME, "Writing %d lines to file", lines);
		writeLines(lines);
	}
	else {
		//std::cout<< "hurlbot: gotta string, wasn't hurl... it was : "<<msg.message() << std::endl;
		ircLog::instance()->logf(FILENAME, "got a string.  it wasn't tome: %s", msg.message().c_str());
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
	ircLog::instance()->logf(FILENAME, "opening tome file: %s", tomeFile);
	std::ofstream tome;
	tome.open(tomeFile, std::ios_base::app);


	ircLog::instance()->logf(FILENAME, "Placing Timestamp");
	tome << std::endl << getEntryTimeStamp() << std::endl;
	
	//count back from the end to the start line
	if(numOfLines > lines.size())
	{
		numOfLines = lines.size();
	}
	int startIndex = lines.size() - numOfLines;
	


	//write those lines to file
	ircLog::instance()->logf(FILENAME, "outputing %d lines", numOfLines);
	//TODO pop off lines before start index
	//make this pop off all remaingin lines in the buffer.
	for(unsigned int i = startIndex; i < lines.size(); ++i)
	{
		tome << lines.at(i) << std::endl;
	}
	

	//close file
	ircLog::instance()->logf(FILENAME, "closing tome file");
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
