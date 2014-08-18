
#include<string>
#include<iostream>

class PluginAttrs{

	std::string _name;
	bool _pm;
	bool _message;
	bool _ipc;
	bool _user_info;
	bool _kick;
	bool _ban;


	public:
	PluginAttrs();
	PluginAttrs(std::string, bool,bool,bool,bool,bool,bool);

	inline std::string name();
	inline bool canPrivateMessage();
	inline bool canChannelMessage();
	inline bool canTalkToPlugins();
	inline bool canGetUserInfo();
	inline bool canKick();
	inline bool canBan();

	friend std::ostream& operator<<(std::ostream& os, const PluginAttrs& attrs);
	friend std::istream& operator>>(std::istream& is, PluginAttrs& attrs);
};
