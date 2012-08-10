#include "OmniPluginInterface.h"
#include <deque>
#include <string>

class tomebot:public OmniPlugin {
	public:
		void onMessage(ircMessage&);
		void onOmniCommConnect(OmniCommChannel*);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();

	private:
		static const unsigned int MAX_LINES = 10;
		static const size_t TIME_BUFF_SIZE = 20;
		static const char tomeFile[30];
		std::deque<std::string> lines;
		PluginUtils* utils;


		void writeLines(int);
		std::string getLineTimeStamp();
		std::string getEntryTimeStamp();

	


};
