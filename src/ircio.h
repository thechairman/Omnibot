#ifndef _IRCIO_
#define _IRCIO_

class ircioCallBack{
	
public:
	virtual void onMessage(std::string) = 0;
};

class ircio{

	public:
		virtual bool open(std::string, int) = 0;
		virtual void close() = 0;
		virtual bool read(std::string&) = 0;
		virtual bool write(std::string&) = 0 ;
		virtual void registerCallBack(ircioCallBack*) = 0;
		virtual unsigned int sleep(unsigned int) = 0;


};


#endif
