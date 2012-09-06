#ifndef _OMNI_CONFIG_PARSER_H_
#define _OMNI_CONFIG_PARSER_H_
#include <string>
#include <vector>

class OmniConfigParser{

	public:
	std::string nick();
	std::string serverName();
	int serverPort();
	std::vector<std::string> channels();
	static OmniConfigParser* instance();
	int  parse();
	
	enum parseCodes
	{
		P_SUCCESS,
		P_INVALID_SECTION,
		P_INVALID_STATE,
		P_INVALID_FIELD,
		P_IO_ERROR
	};


	protected:
	OmniConfigParser();
	

	private:

	static OmniConfigParser* _instance;

	static const std::string OMNI_CONFIG_FILE_NAME;
	static const std::string NICKS;
	static const std::string SERVER;
	static const std::string CHANNELS;

	enum parseStates
	{
		PS_INVALID,
		PS_NICK,
		PS_SERVER,
		PS_CHANNELS
	};
	
	std::string _nick;
	std::string _serverName;
	int _serverPort;
	std::vector<std::string> _channels;
	
};
#endif
