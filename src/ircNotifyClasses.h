
class ircUser{
private:
	std::string nick;
	int userId;
public:	
	ircUser();
	ircUser(std::string nick_, int userId);
	std::string nick();
	int userId();
}

class ircMessage{
private:
	std::string message;
	ircUser* user;
public:
	ircMessage();
	ircMessage(ircUser* user_, std::string Message);
	std::string message();
	ircUser* user();
}

class ircEvent{

}

