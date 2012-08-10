#include "OmniPluginInterface.h"
#include <deque>
#include <string>

class hURLbot:public OmniPlugin {
	public:
		void onMessage(ircMessage&);
		void onOmniCommConnect(OmniCommChannel*);
		bool init(PluginUtils*);
		void wrapUp();
		std::string name();

	private:
		static const unsigned int MAX_URLS = 50;
		static const double CULL_INTERVAL = 600;
		static const double TWENTY_FOUR_HOURS = 86400;
		time_t lastCulling;
		struct stampedUrl{
			time_t timeStamp;
			std::string url;

		};
		std::deque<stampedUrl> urls;
		PluginUtils* utils;

		void cullStaleUrls();


};
