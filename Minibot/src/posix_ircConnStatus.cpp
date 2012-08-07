#include <unistd.h>
#include <iostream>

#include "posix_ircConnStatus.h"
#include "ircTypes.h"

posix_ircConnStatus::posix_ircConnStatus()
{
	pthread_mutex_init(&pingMux, NULL);
	sem_init(&connSetupSem, 0, 0);
}
posix_ircConnStatus::~posix_ircConnStatus()
{
	pthread_mutex_destroy(&pingMux);
	sem_destroy(&connSetupSem);
}

int posix_ircConnStatus::init()
{
	_state = CS_IDLE;
	time(&lastPing);
	//verify all mutexes are ready togo
	_monitorRunning = true;
	pthread_create(&monitor, NULL, posix_ircConnStatus::monitorThread, (void*) this);
	return 0;
}

void posix_ircConnStatus::registerCallBack(ircConnStatusCB* irc)
{
	interface = irc;
}

connState_t posix_ircConnStatus::state()
{
	return _state;
}

void posix_ircConnStatus::pingRcvd()
{
	pthread_mutex_lock(&pingMux);
	time(&lastPing);
	pthread_mutex_unlock(&pingMux);
}

int posix_ircConnStatus::waitOnConnect()
{
	sem_wait(&connSetupSem);
	return connReturn;
}

void posix_ircConnStatus::connected()
{
	connReturn = CONNECT_SUCCESS;
	_state = CS_CONNECTED;
	sem_post(&connSetupSem);
}
void posix_ircConnStatus::registered()
{
	connReturn = CONNECT_SUCCESS;
	_state = CS_REGISTERED;
	sem_post(&connSetupSem);
}
void posix_ircConnStatus::nameCollision()
{
	connReturn = NICK_COLLISION;
	_state = CS_FAILED;
	sem_post(&connSetupSem);
}

void* posix_ircConnStatus::monitorThread(void* stat)
{
	posix_ircConnStatus* connStat = (posix_ircConnStatus*) stat;

	while(connStat->_monitorRunning){	

		if(!connStat->validatePings())
		{
			//roll back the connection status and let the interface know
			connStat->interface->onConnectionDeath();
			connStat->_monitorRunning = false;
			continue;
		}

		sleep(MONT_INTV);
	}

	return 0;
	
}
bool posix_ircConnStatus::validatePings()
{
	pthread_mutex_lock(&pingMux);
	
	time_t t;
	time(&t);
	double dif = difftime(lastPing, t);

	std::cout << "ircConnStatus: ping diff = " << dif << std::endl;

	if(dif > PING_TIME_OUT)
	{	
		return false;
	}
	return true;
}
