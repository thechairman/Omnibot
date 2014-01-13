#ifndef _OMNI_CONFIG_PARSER_H_
#define _OMNI_CONFIG_PARSER_H_
#include <string>
#include <vector>

#include "OmniCfgReader.h"

class OmniConfigParser{

	public:


	enum parseCodes
	{
		P_SUCCESS,
		P_INVALID_SECTION,
		P_INVALID_STATE,
		P_INVALID_FIELD,
		P_INVALID_VALUE,
		P_IO_ERROR
	};

	enum pluginLoadModes
	{
		PLM_STATIC,
		PLM_DYNAMIC
	};


	std::string nick();
	std::string serverName();
	int serverPort();
	std::vector<std::string> channels();
	std::vector<std::string> plugins();
	bool autoReconnect();
	int maxReconnRetries();
	pluginLoadModes pluginLoadMode();
	std::string pluginDirectory();	
	static OmniConfigParser* instance();
	int  parse();	


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
		PS_AUTHENTICATION,
		PS_RECONNECTION,
		PS_PLUGIN_LOADING
	};

	OmniCfgReader _reader;

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
	
	//section headers
	static const std::string OMNI_CONFIG_FILE_NAME;


	
	//config fields
	std::string _nick;
	std::string _serverName;
	int _serverPort;
	std::vector<std::string> _channels;
	std::vector<std::string> _plugins;
	bool _autoReconnect;
	int _maxReconnRetries;
	std::string _pluginDir;
	pluginLoadModes _pluginLoadMode;

	class ConfigSection
	{
		public:
			virtual bool isSection(std::string) = 0;
			virtual parseCodes loadEntry(std::string)= 0;
			virtual void clear() = 0;
	};

	class NickSection :public ConfigSection
	{
		public:
		NickSection(OmniCfgReader&, OmniConfigParser*);
		bool isSection(std::string);
		parseCodes loadEntry(std::string);
		void clear();

		private:
		static const std::string THIS_SECTION;//NICKS;
		static const std::string DEFAULT_NICK;
		OmniCfgReader& _reader;
		OmniConfigParser* _parser;

	};
	class ServerSection:public ConfigSection
	{
		public:
		ServerSection(OmniCfgReader&, OmniConfigParser*);
		bool isSection(std::string);
		parseCodes loadEntry(std::string);
		void clear();

		private:
		static const std::string THIS_SECTION;//SERVER;
		static const std::string DEFAULT_SERVER;
		static const int DEFAULT_PORT;
		OmniCfgReader& _reader;
		OmniConfigParser* _parser;

	};
	class ChannelsSection:public ConfigSection
	{
		public:
		ChannelsSection(OmniCfgReader&, OmniConfigParser*);
		bool isSection(std::string);
		parseCodes loadEntry(std::string);
		void clear();

		private:
		static const std::string THIS_SECTION;//CHANNELS;
		OmniCfgReader& _reader;
		OmniConfigParser* _parser;

	};
	class PluginsSection:public ConfigSection
	{
		public:
		PluginsSection(OmniCfgReader&, OmniConfigParser*);
		bool isSection(std::string);
		parseCodes loadEntry(std::string);
		void clear();

		private:
		static const std::string THIS_SECTION; //PLUGINS;
		static const std::string DEFAULT_PLUGIN_DIR;
		static const pluginLoadModes DEFAULT_LOAD_MODE;
		OmniCfgReader& _reader;
		OmniConfigParser* _parser;

	};
	class AuthSection:public ConfigSection
	{
		public:
		AuthSection(OmniCfgReader&, OmniConfigParser*);
		bool isSection(std::string);
		parseCodes loadEntry(std::string);
		void clear();

		private:
		static const std::string THIS_SECTION; //AUTHENTICATION;
		OmniCfgReader& _reader;
		OmniConfigParser* _parser;

	};
	class ReconnSection:public ConfigSection
	{
		public:
		ReconnSection(OmniCfgReader&, OmniConfigParser*);
		bool isSection(std::string);
		parseCodes loadEntry(std::string);
		void clear();

		private:
		static const std::string THIS_SECTION; //RECONNECTIONS;
		static const bool DEFAULT_AUTOCONNECT;
		static const int  DEFAULT_RETRIES;
		OmniCfgReader& _reader;
		OmniConfigParser* _parser;

	};
};

#endif
