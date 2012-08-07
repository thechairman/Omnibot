#ifndef _POSIX_IRCCONNSTATUS_H_
#define _POSIX_IRCCONNSTATUS_H_

#include<ctime>
#include<pthread.h>
#include<semaphore.h>

#include "ircTypes.h"
#include "ircConnStatus.h"
class posix_ircConnStatus: public ircConnStatus
{
	public:

		posix_ircConnStatus();
		~posix_ircConnStatus();
		int init();
		void registerCallBack(ircConnStatusCB*);

		connState_t state();

		void pingRcvd();
	 	int  waitOnConnect();
		void connected();
		void registered();
		void nameCollision();

	private:
		//private constants
		static const unsigned int MONT_INTV = 5;
		static const double PING_TIME_OUT = 120;

		bool _monitorRunning;

		connState_t _state;
		ircConnStatusCB* interface;
		pthread_t monitor;

		time_t lastPing;
		pthread_mutex_t pingMux;

		sem_t connSetupSem;
		int connReturn;

		static void* monitorThread(void*);
		bool validatePings();

};
#endif

