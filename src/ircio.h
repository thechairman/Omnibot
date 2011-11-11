#ifndef _IRCIO_
#define _IRCIO_

class ircio{

	public:
		virtual bool open(std::string, int) = 0;
		virtual void close() = 0;
		virtual bool read(std::string&) = 0;
		virtual bool write() = 0 ;
		virtual void registerCallBack(ircioCallBack*) = 0;


};

class ircioCallBack{
	virtual void onMessage(std::string) = 0;
}

#endif
