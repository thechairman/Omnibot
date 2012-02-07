#include <queue>
#include <pthread.h>

class OmniThread{
public:

	OmniThread();

	static const int SUCCESS = 0;
	static const int THREAD_ALREADY_RUNNING = -1;
	static const int SYS_THREAD_ERR = -2;

	class OmniMutex{};
	void callBack(void*(void*));
	void arg(void*);
	int start();
	void join();

	void addTask(void*(void*), void*);

	bool isRunning();

	private:
	bool threadStarted;
	static void* OmniThreadCallback(void*);

	struct callBackData{
		int _argc;	
		void** _argv;
		void* (*_callback)(void*);
	}_data;

	pthread_t _thread;

	std::queue<callBackData> _tasks;
};

