#include <queue>
#include <pthread.h>

class OmniMutex{
	private:
		pthread_mutex_t _mutex;

	public:
		OmniMutex();
		~OmniMutex();
		bool lock();
		bool unlock();
		bool tryLock();

		

};
class OmniThread{
public:

	OmniThread();

	static const int SUCCESS = 0;
	static const int THREAD_ALREADY_RUNNING = -1;
	static const int SYS_THREAD_ERR = -2;

	void callBack(void*(void*));
	void arg(void*);
	int start();
	void join();

	int addTask(void*(void*), void*);

	bool isRunning();

	private:
	bool threadStarted;
	int createThread();
	static void* OmniThreadCallback(void*);

	struct callBackData{
		int _argc;	
		void** _argv;
		void* (*_callback)(void*);
	}_data;

	pthread_t _thread;

	std::queue<callBackData> _tasks;
	OmniMutex _taskMutex;
	OmniMutex _stateMutex;
};

