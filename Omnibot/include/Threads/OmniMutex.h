#ifndef _OMNIMUTEX_H_
#define _OMNIMUTEX_H_

class OmniMutex{
	public:
		virtual bool lock() =0;
		virtual bool unlock() =0;
		virtual bool tryLock() =0;
		static OmniMutex* create();

};

#endif
