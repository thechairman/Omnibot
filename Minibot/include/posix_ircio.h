#ifndef _POSIX_IRCIO_
#define _POSIX_IRCIO_


#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#include<pthread.h>

#include<vector>
#include<string>
#include<iostream>

#include"ircio.h"

class posix_ircio : public ircio{

	public:
		virtual bool  open(std::string server, int port);
		virtual void close();
		virtual bool read(std::string&);
		virtual bool write(std::string&);
		virtual void registerCallBack(ircioCallBack*);
		virtual unsigned int sleep(unsigned int);

	private:
		static const long SELECT_SECS = 0;
		static const long SELECT_NSECS = 10000;
		static void* startListening(void* ptr);
		virtual void listen();
		virtual void onReceive(std::string);
		int socket;
		static const int BUFSIZE = 255;
		pthread_t listenerThread;

		//should probably add mutex for these
		//at some point
		bool isOpen;
		std::vector<ircioCallBack*> callbacks;


};

#endif
