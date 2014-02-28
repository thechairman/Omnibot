#ifndef NUMBERWANG_BOT_H_
#define NUMBERWANG_BOT_H_


#include <cstdlib>
#include <string>

#include "OmniPluginInterface.h"

class numberwangBot:public OmniPlugin {

	public:

		void onMessage(ircMessage&);
		void onOmniCommConnect(OmniCommChannel*);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();

	private:
		PluginUtils* _utils;


};

#endif
