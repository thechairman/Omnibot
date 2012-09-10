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
	std::vector<std::string> plugins();
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

	enum parseStates
	{
		PS_INVALID,
		PS_NICK,
		PS_SERVER,
		PS_CHANNELS,
		PS_PLUGINS,
		PS_AUTHENTICATION
	};

#if 0
	class fields
	{	
		public:
			parseCodes handleLine(std::string);
			parseStates state();
		private:
			parseStates _state;
			static const std::string NICKS;
			static const std::string SERVER;
			static const std::string CHANNELS;
			static const std::string PLUGINS;
			static const std::string AUTHENTICATION;

	
	};
	class nicks
	{
		public:
			parseCodes handleLine(std::string);
			std::string nick();
			std::string alt();

		private:
			std::string _nick;
			std::string _alt;
	};
	class server
	{
		public:
			parseCodes handleLine(std::string);
			std::string name();
			int port();
		private:
			std::string _name;
			int _port;

			static const std::string NAME;
			static const std::string PORT;
	};
	class channels
	{
		public:
			parseCodes handleLine(std::string);
			std::vector& channels();
		private:
			std::vector<std::string> _channels;
			

	};
	class plugins
	{
		public:
			parseCodes handleLine(std::string);
			std::vector& plugins();
		private:
			std::vector<std::string> _plugins;
	};

	class auththentication
	{
		public:
			parseCodes handleLine(std::string);
	};
#endif	

	static OmniConfigParser* _instance;

	static const std::string OMNI_CONFIG_FILE_NAME;
	static const std::string NICKS;
	static const std::string SERVER;
	static const std::string CHANNELS;
	static const std::string PLUGINS;
	static const std::string AUTHENTICATION;

	
	std::string _nick;
	std::string _serverName;
	int _serverPort;
	std::vector<std::string> _channels;
	std::vector<std::string> _plugins;;
	
};
#endif
