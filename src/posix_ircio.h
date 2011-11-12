#ifndef _POSIX_IRCIO_
#define _POSIX_IRCIO_


#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#include<pthread.h>

#include<vector>

#include"ircio.h"

class posix_ircio : public ircio{

	public:
		virtual bool  open(std::string server, int port);
		virtual void close();
		virtual bool read(std::string&);
		virtual bool write(std::string&);
		virtual void registerCallBack(ircioCallBack*);

	private:
		virtual void listen(void* ptr);
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
