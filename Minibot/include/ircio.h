#ifndef _IRCIO_
#define _IRCIO_

#include <string>

class ircioCallBack{
	
public:
	virtual void onMessage(std::string) = 0;
};

class ircio{

	public:
		static ircio* create();
		virtual bool open(std::string, int) = 0;
		virtual void close() = 0;
		virtual bool read(std::string&) = 0;
		virtual bool write(std::string&) = 0 ;
		virtual void registerCallBack(ircioCallBack*) = 0;
		virtual unsigned int sleep(unsigned int) = 0;


};


#endif
