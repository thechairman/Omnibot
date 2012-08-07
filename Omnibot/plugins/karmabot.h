#include "OmniPluginInterface.h"

class karmabot: public OmniPlugin{


	private:
		std::map<unsigned int, int> _karma;

		void decrementKarma(ircUser*);
		void incrementKarma(ircUser*);

		void loadKarma();
		void saveKarma();
};

