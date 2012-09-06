#include "posix_OmniBlocker.h"

posix_OmniBlocker::posix_OmniBlocker():_numBlocked(0)
{
	sem_init(&_blockingSem, 0, 0);
	sem_init(&_numSem, 0, 1);
}
posix_OmniBlocker::~posix_OmniBlocker()
{
	sem_destroy(&_blockingSem);
	sem_destroy(&_numSem);
}

void posix_OmniBlocker::block()
{
	sem_wait(&_numSem);
	++_numBlocked;
	sem_post(&_numSem);

	sem_wait(&_blockingSem);


}

void posix_OmniBlocker::release()
{
	sem_wait(&_numSem);

	for(int i = 0; i < _numBlocked; ++i)
	{
		sem_post(&_blockingSem);
	}

	sem_post(&_numSem);
}

unsigned int posix_OmniBlocker::numberWaiting()
{
	return _numBlocked;
}
