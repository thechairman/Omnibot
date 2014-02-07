
#ifndef _OMNI_BLOCKER_H_
#define _OMNI_BLOCKER_H_


class OmniBlocker {
	public:
	virtual void block() =0;
	virtual void release() =0;
	virtual unsigned int numberWaiting() =0;
	static OmniBlocker* create();
};

#endif
