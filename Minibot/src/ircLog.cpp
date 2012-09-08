
#include <cstdio>
#include <ctime>
#include <cstring>

#include "ircLog.h"

#if 0
class ircLog {

	public:
		static ircLog* instance();

		ircLog();
		void setOutputStream(std::ostream);
		void logf(std::string, std::string, ...);


	private:
		std::ostream _stream;
		static ircLog* _instance;
		static const LOG_BUFF_SIZE = 512;
	
};

#endif 

ircLog* ircLog::_instance = NULL;

ircLog::ircLog():_stream(&std::cout)
{}

ircLog* ircLog::instance()
{
	if(_instance == NULL)
	{
		_instance = new ircLog();
	}

	return _instance;
}

void ircLog::setOutputStream(std::ostream stream)
{
	_stream = &stream;
}

void ircLog::logf(std::string fname, std::string format, ...)
{
	time_t currentTime;
	char timeBuff[20];

	memset(timeBuff, 0, sizeof(timeBuff));

	time(&currentTime);
	strftime(timeBuff, 20, "%x %X", localtime(&currentTime));
	
			
	*_stream << timeBuff << " - " << fname << ": ";
	

	char* buffer = new char[format.size() + LOG_BUFF_SIZE];
	memset(buffer, 0, format.size() + LOG_BUFF_SIZE);

	va_list args;

	va_start(args, format);

	vsprintf(buffer, format.c_str(), args);

	va_end(args);

	*_stream << buffer << std::endl;

	delete buffer;

}
