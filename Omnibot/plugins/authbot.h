#include "OmniPluginInterface.h"
#include <deque>
#include <string>

class authbot:public OmniPlugin {
	public:
		void onMessage(ircMessage&);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();

	private:
		PluginUtils* utils;


};
