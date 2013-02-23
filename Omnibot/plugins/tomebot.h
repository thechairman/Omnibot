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

		static const size_t CMD_LEN = 5; 		/**< the lenght of "!tome"*/
		static const unsigned int MAX_LINES = 10;	/**< the maximum lines to buffer*/
		static const size_t TIME_BUFF_SIZE = 20;	/**< size of the buffer used to create the time stamp*/

		static const char tomeFile[30];
		std::deque<std::string> lines;
		PluginUtils* utils;


		void writeLines(int);
		std::string getLineTimeStamp();
		std::string getEntryTimeStamp();

	


};
