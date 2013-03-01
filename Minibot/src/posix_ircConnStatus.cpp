#include <unistd.h>
#include <iostream>

#include "posix_ircConnStatus.h"
#include "ircTypes.h"
#include "ircLog.h"

const std::string FILENAME = "posix_ircConnStat.cpp";

posix_ircConnStatus::posix_ircConnStatus():
	_monitorRunning(false),
	_state(CS_IDLE),
	interface(NULL),
	connReturn(CONNECT_SUCCESS),
	_timerExists(false)
	
{
	pthread_mutex_init(&pingMux, NULL);
	sem_init(&connSetupSem, 0, 0);

}


posix_ircConnStatus::~posix_ircConnStatus()
{
	_monitorRunning = false;

	switch(_mode)
	{
		case MM_THREAD:
			//should stop the thread and wait for it to end
			break;

		case MM_TIMER:
			if(_timerExists)
			{
				stopTimer();
				sleep(MONT_INTV);
				timer_delete(_monitorTimer);
				_timerExists = false;
			}
			break;
	}

	pthread_mutex_destroy(&pingMux);
	sem_destroy(&connSetupSem);

}


/**Function (re)initinalizes the connection status making it ready to attempt a connection
 * @return returns an error code related to the point of failure in the initialzation 
 */
int posix_ircConnStatus::init()
{
	int rc = 0;
	_state = CS_IDLE;	//initilize state
	time(&lastPing);	//set the last ping to now

	_mode = MM_TIMER;


	//TODO verify all mutexes are ready togo
	
	_monitorRunning = true; //mark the monitor as running

	if(_mode == MM_THREAD)
	{
		pthread_create(&_monitor, NULL, posix_ircConnStatus::monitorThread, (void*) this);
	}
	else if (_mode == MM_TIMER)
	{
		rc = initTimer();
	}
	else
	{
		//something broke;
		rc = -1;
	}
	return rc;
}

/**This function sets the class to notify of stat events
 *
 * @param[in] a pointer to a class implementing the call back interface
 * @return void
 */
void posix_ircConnStatus::registerCallBack(ircConnStatusCB* irc)
{
	interface = irc;
}

/**This function retrieves the current connection state.
 * @return the current connection state
 */
connState_t posix_ircConnStatus::state()
{
	return _state;
}

/**This function is called to update the ping time stamp*/
void posix_ircConnStatus::pingRcvd()
{
	ircLog::instance()->logf(FILENAME, "Recieved ping/message");
	pthread_mutex_lock(&pingMux);
	time(&lastPing);
	pthread_mutex_unlock(&pingMux);
}

/**This function hangs until a connection reply is received
 * @return CONNECT_SUCCESS or indicates failure by return a connSetup_t value
 */ 
int posix_ircConnStatus::waitOnConnect()
{
	sem_wait(&connSetupSem);
	return connReturn;
}

/**This function is called when a successful connection occurs
 *
 * This function is called when a successful connection is established with the ircserver.
 * It then unblocks the thread in the process of setting up the connection and registering
 * the user.  It also sets the appropriate return code for waitOnConnect() and updates the state
 */
void posix_ircConnStatus::connected()
{
	connReturn = CONNECT_SUCCESS;
	_state = CS_CONNECTED;
	sem_post(&connSetupSem);
}
/**This function is called when the user successfully registers
 *
 * This function is called when a message indicating a successful registration 
 * with the ircserver is received. It then unblocks the thread in the process of 
 * setting up the connection and registering the user.  It also sets the appropriate
 * return code for waitOnConnect() and updates the state.
 */
void posix_ircConnStatus::registered()
{
	connReturn = CONNECT_SUCCESS;
	_state = CS_REGISTERED;
	sem_post(&connSetupSem);
}

/**This funciton is called when a nick collision occurs during the registration process.
 *
 * This function is called if a user with the same nick is already present on the server.
 * It sets the return code of waitOnConnect to NICK_COLLISION, before setting the state to
 * CS_FAILED and unblocking waitOnConnect()
 */
void posix_ircConnStatus::nameCollision()
{
	connReturn = NICK_COLLISION;
	_state = CS_FAILED;
	sem_post(&connSetupSem);
}
/**This is the monitor thread used for MM_THREAD
 *
 * 
 * 
 */
void* posix_ircConnStatus::monitorThread(void* stat)
{
	posix_ircConnStatus* connStat = (posix_ircConnStatus*) stat;

	while(connStat->_monitorRunning){	

		connStat->monitor();

		sleep(MONT_INTV);
	}

	return 0;
	
}

void posix_ircConnStatus::monitorTick(sigval val)
{
	
	posix_ircConnStatus* connStat = (posix_ircConnStatus*) val.sival_ptr;

	if(connStat->_monitorRunning)
	{
		connStat->monitor();
	}

	if(!connStat->_monitorRunning)
	{
		
		// stop the timer
		//connStat->_time.it_value.tv_sec = 0;
		//timer_settime(connStat->_monitorTimer, 0, &connStat->_time, NULL);
		connStat->stopTimer();
	}
}


void posix_ircConnStatus::monitor()
{
	 ircLog::instance()->logf(FILENAME, "Evaluating connection state...");
	 ircLog::instance()->logf(FILENAME, "Current State is %d", (int)_state);

	switch(_state)
	{

		case CS_IDLE:
			//ircLog::instance()->logf(FILENAME, "executing IDLE analysis");
			break;

		case CS_FAILED:
			//ircLog::instance()->logf(FILENAME, "Executing FAILED analysis");
			stopTimer();
			interface->onConnectionDeath();
			break;

		case CS_CONNECTED:
			//ircLog::instance()->logf(FILENAME, "Executing CONNECTED analysis");
			break;

		case CS_REGISTERED:
			//ircLog::instance()->logf(FILENAME, "Exectuing REGISTERED analysis");
			if(!validatePings())
			{
				//roll back the connection status and let the interface know
				ircLog::instance()->logf(FILENAME, "ping time out: %d seconds",
					PING_TIME_OUT);
				_monitorRunning = false;
				stopTimer();
				interface->onConnectionDeath();
				return;
			}
			break;

		default:
			break;
			
	}
}

bool posix_ircConnStatus::validatePings()
{
	bool rc;
	pthread_mutex_lock(&pingMux);
	
	time_t t;
	time(&t);
	double dif = difftime(t, lastPing);

	//std::cout << "ircConnStatus: ping diff = " << dif << std::endl;
	ircLog::instance()->logf(FILENAME, "ping difference = %f", dif);

	rc = true;
	if(dif > PING_TIME_OUT)
	{	
		ircLog::instance()->logf(FILENAME, "Ping time out");
		rc = false;
	}

	pthread_mutex_unlock(&pingMux);

	return rc;
}

int posix_ircConnStatus::initTimer()
{
	_sigev.sigev_notify = SIGEV_THREAD;
	_sigev.sigev_notify_function = &monitorTick;
	_sigev.sigev_value.sival_ptr = (void*) this;

	_time.it_interval.tv_sec = MONT_INTV;
	_time.it_interval.tv_nsec = 0;

	_time.it_value.tv_sec = MONT_INTV;
	_time.it_value.tv_nsec = 0;

	int rc = 0;

	if(!_timerExists)
	{
		rc = timer_create(CLOCK_MONOTONIC, &_sigev, &_monitorTimer);
	}
	
	if(rc){
		return rc;
	}

	_timerExists = true;
	
	rc = timer_settime(_monitorTimer, 0, &_time, NULL);

	return rc;

}

void posix_ircConnStatus::stopTimer()
{
	if(_mode == MM_TIMER)
	{
		_time.it_value.tv_sec = 0;
		_time.it_value.tv_nsec = 0;
		_time.it_interval.tv_sec = 0;
		_time.it_interval.tv_nsec = 0;
			
		timer_settime(_monitorTimer, 0, &_time, NULL);
	}
}
void posix_ircConnStatus::connectionIoError()
{
	_state = CS_FAILED;
}
