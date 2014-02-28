#include "OmniPluginInterface.h"
#include <map>
#include <utility>
#include <string>

class wordsbot:public OmniPlugin {
	public:
		void onMessage(ircMessage&);
		void onOmniCommConnect(OmniCommChannel*);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();

	private:
		const static int FIRST = 0;
		const static int SECOND = 1;
		const static int NUM_LINES = 8;
		const static int MAX_LINE_LEN = 25;
		
		const static std::string WORDS_FNAME;
		const static std::string UNSPEAKABLE_WORDS_FNAME;
		const static std::string LINES[NUM_LINES][MAX_LINE_LEN];

		std::map<std::string, std::string> _words;
		std::vector<std::string>* _unspeakable_words;
		PluginUtils* utils;

		std::string getString(const std::string&, const std::string&);
		bool addString(const std::string&);
		std::pair<std::string, std::string>* searchString(const std::string&);
		bool proc();
		void saveWords();
		void loadWords();
		void loadUnspeakables();
		void saveUnspeakables();
		bool isUnspeakable(std::string);


};
