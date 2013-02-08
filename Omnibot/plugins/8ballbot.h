#ifndef EIGHT_BALL_BOT_H_
#define EIGHT_BALL_BOT_H_


#include <cstdlib>
#include <string>

#include "OmniPluginInterface.h"

class eightBallBot:public OmniPlugin {

	public:

		void onMessage(ircMessage&);
		void onOmniCommConnect(OmniCommChannel*);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();

	private:
		static const int NUM_OF_STATEMENTS = 20;
		static const std::string replys[NUM_OF_STATEMENTS];

	//	void sendReply();
		int getRandom();

		PluginUtils* _utils;


};

#endif
