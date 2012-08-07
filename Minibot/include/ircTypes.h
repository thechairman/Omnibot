#ifndef _IRCTYPES_H_
#define _IRCTYPES_H_


enum connSetup_t
{
	CONNECT_SUCCESS = 0,
	NICK_COLLISION
};

enum connState_t
{
	CS_IDLE,
	CS_FAILED,
	CS_CONNECTED,
	CS_REGISTERED
};

#endif
