#ifndef _IRC_USERAUTH_
#define _IRC_USERAUTH_
#include "ircInterface.h"
#include "ircUserDB.h"
#include "ircNotifyClasses.h"
#include <string>

class ircInterface;

class ircUserAuth
{
	public:
		enum AuthType
		{	
			AUTH_NONE,
			AUTH_LOCAL,
			AUTH_NICKSERV
		};

		ircUserAuth(ircInterface*, ircUserDB*);
		virtual ~ircUserAuth();
		virtual void nickServInfo(std::string);
		virtual void addUser(std::string);
		virtual ircUserAuth::AuthType authMethod();
		void authMethod(AuthType);
		void nickPrefixes(std::string);


	protected:
		AuthType _authMethod;
		ircUserDB* _userDB;
		ircInterface* _irc;
		std::string _nickModePrefixes;

};
#endif
