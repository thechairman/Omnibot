#ifndef _H_PTHREAD_THREADPOOL_
#define _H_PTHREAD_THREADPOOL_
#include <vector>

#include "OmniThreadPool.h"
#include "pthread_PooledThread.h"

class pthread_ThreadPool: public OmniThreadPool
{
	public:
		OmniPooledThread* requestThread();
		void releaseThread(OmniPooledThread*);

	private:
		int addThread(pthread_PooledThread*&);
		int removeThread(pthread_PooledThread&);
		std::vector<pthread_PooledThread*> _threadPool;

		typedef std::vector<pthread_PooledThread*>::iterator Pool_iter;

};

#endif
