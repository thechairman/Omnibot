#ifndef _BASHBOT_
#define _BASHBOT_

#include "OmniPluginInterface.h"
#include <vector>
#include <deque>
#include <map>
#include<sstream>
#include "Threads/OmniThreadedClass.h"
#include "Threads/OmniThread.h"

class bashbot:public OmniPlugin
{
	public: 
		void onMessage(ircMessage&);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();

	private:

		PluginUtils* utils;
		std::map<std::string, std::string> entities;
		void initHtmlMap();

		struct bashQuote{
			std::string num;
			std::vector<std::string> lines;
		};

		typedef std::vector<char> bashBuffer; 

		bool refreshCache();
		std::string search(std::string);
		bashQuote nextBash();
		bashQuote bashNum(std::string);
		bashBuffer* webget(std::string);
		

		void printQuote(bashQuote, std::string);




		std::deque<bashQuote> cache;

		//parsing subfucntions
		bashQuote parseQuote(std::stringstream&);
		std::string getNum(std::string);
		std::string getInitialLine(std::string);
		std::vector<std::string> getBashNums(std::stringstream&);
		std::string trimBodyLine(std::string);
		std::string unescapehtml(std::string, int);


		static const std::string baseURL;
		static const int GLBUFFERSIZE = 255;
		static const size_t CACHE_MIN = 5;
		static const unsigned int SPAM_INTV = 500000;

		static size_t writeMemCallback(void*, size_t, size_t, void*);

		class cachebuilder: public OmniThreadedClass
		{
			public:
				cachebuilder(bashbot*);
				void runA();
				void runB();
				void runC();
				void runD();

			private:
				bashbot* bot;

		};

		OmniThread* cacheThread;

};

#endif
