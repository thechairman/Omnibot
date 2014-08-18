
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

	inline std::string name(){return _name;}
	inline bool canPrivateMessage(){return _pm;}
	inline bool canChannelMessage(){return _message;}
	inline bool canTalkToPlugins(){return _ipc;}
	inline bool canGetUserInfo(){return _user_info;}
	inline bool canKick(){return _kick;}
	inline bool canBan(){return _ban;}

	friend std::ostream& operator<<(std::ostream& os, const PluginAttrs& attrs);
	friend std::istream& operator>>(std::istream& is, PluginAttrs& attrs);
};
