#include"ircConnStatus.h"
#include"posix_ircConnStatus.h"

ircConnStatus* ircConnStatus::_instance = NULL;

ircConnStatus::ircConnStatus(){}

ircConnStatus* ircConnStatus::instance(){
	if(_instance == NULL)
	{
		_instance = (ircConnStatus*) new posix_ircConnStatus();
	}

	return _instance;

}
