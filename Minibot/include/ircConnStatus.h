#ifndef _IRCCONNSTATUS_H_
#define _IRCCONNSTATUS_H_

#include "ircTypes.h"

class ircConnStatusCB
{
	public:
		virtual void onConnectionDeath() = 0;
};

class ircConnStatus
{
	public:
		virtual int init() =0;
		virtual void pingRcvd()=0;
	 	virtual int  waitOnConnect()=0;
		virtual void connected()=0;
		virtual void registered() =0;
		virtual void nameCollision() =0;
		virtual connState_t state() =0;
		virtual void registerCallBack(ircConnStatusCB* ) = 0;

		virtual void connectionIoError() =0;

		static ircConnStatus* instance();

	protected:
		static ircConnStatus* _instance;
		ircConnStatus();
};
#endif

