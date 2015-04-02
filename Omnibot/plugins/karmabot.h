#include "OmniPluginInterface.h"
#include "ircNotifyClasses.h"

#include<map>
#include<string>

class karmabot: public OmniPlugin{


	public:
		void onMessage(ircMessage&);
		void onOmniCommConnect(OmniCommChannel*);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();


	private:
		//std::map<unsigned int, int> _karma;
		//
		//void decrementKarma(ircUser*);
		//void incrementKarma(ircUser*);
		
		const static std::string KARMA_FNAME;
		const static std::string INCREMENTOR;
		const static std::string DECREMENTOR;
		const static std::string COMMAND_STR;
		const static size_t COMMAND_LEN = 6;
		
		PluginUtils* utils;
		std::map<std::string, int> _karma;

		void decrementKarma(std::string);
		void incrementKarma(std::string);

		void loadKarma();
		void saveKarma();
		void stripString(std::string&,  const std::string&);
};

