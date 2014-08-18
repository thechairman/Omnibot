#include<PluginAttrs.h>

PluginAttrs::PluginAttrs():
	_name("default"),
	_pm(true),
	_message(true),
	_ipc(false),
	_user_info(false),
	_kick(false),
	_ban(false)
{}

PluginAttrs::PluginAttrs(std::string name, bool pm, bool msg, bool ipc,
		bool user_info, bool kick, bool ban):
	_name(name),
	_pm(pm),
	_message(msg),
	_ipc(ipc),
	_user_info(user_info),
	_kick(kick),
	_ban(ban)
{}

inline std::string PluginAttrs::name(){return _name;}
inline bool PluginAttrs::canPrivateMessage(){return _pm;}
inline bool PluginAttrs::canChannelMessage(){return _message;}
inline bool PluginAttrs::canTalkToPlugins(){return _ipc;}
inline bool PluginAttrs::canGetUserInfo(){return _user_info;}
inline bool PluginAttrs::canKick(){return _kick;}
inline bool PluginAttrs::canBan(){return _ban;}


std::ostream& operator<<(std::ostream& os, const PluginAttrs& attrs)
{
	os << attrs._name;
	os << attrs._pm;
	os << attrs._message;
	os << attrs._ipc;
	os << attrs._user_info;
	os << attrs._kick;
	os << attrs._ban;
	return os;
}
std::istream& operator>>(std::istream& is, PluginAttrs& attrs)
{
	is >> attrs._name;
	is >> attrs._pm;
	is >> attrs._message;
	is >> attrs._ipc;
	is >> attrs._user_info;
	is >> attrs._kick;
	is >> attrs._ban;
	return is;
}
