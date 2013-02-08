#ifndef _POSIX_IRCCONNSTATUS_H_
#define _POSIX_IRCCONNSTATUS_H_

#include<ctime>
#include<pthread.h>
#include<semaphore.h>
#include<signal.h>

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

		void connectionIoError();

	private:

		enum MonitorMode_t
		{
			MM_THREAD,  		/**<use a constantly running and sleeping thread*/
			MM_TIMER		/**<use a posix timer with a callback */

		};

		//private constants
		static const unsigned int MONT_INTV = 5;	/**<time between evaluation in secs*/
		static const double PING_TIME_OUT = 120;	/**<connection time out*/

		MonitorMode_t _mode;		/**<evaluation mode*/		

		bool _monitorRunning;		

		connState_t _state;		/**<current connection state*/
		ircConnStatusCB* interface;	/**<class to notify of failure*/
		pthread_t _monitor;		/**< monitor thread reference*/

		time_t lastPing;		/**<time stamp of last ping*/
		pthread_mutex_t pingMux;	/**<mux for for the ping timestamp*/

		//these varibles are used for blocking replys during connection setup
		sem_t connSetupSem;		/**<sem used for reply blocking*/
		int connReturn;			/**<return value set by reply*/

		//timer stuff
		bool _timerExists;
		sigevent _sigev;
		itimerspec _time;
		//sigaction _sact;
		timer_t _monitorTimer;

		static void* monitorThread(void*);
		static void monitorTick(sigval);

		bool validatePings();
		int initTimer();
		void stopTimer();
		void monitor();

};
#endif

