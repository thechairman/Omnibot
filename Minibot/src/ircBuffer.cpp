#include "ircBuffer.h"
#include "ircLog.h"

const std::string FILENAME = "ircBuffer.cpp";

std::string ircBuffer::nextMessage()
{
	std::string msg = _msgs.front();
	_msgs.pop_front();
	return msg;
}

bool ircBuffer::hasNextMessage()
{
	bool rc = false;

	if(_msgs.size() > 0)
		rc = true;
	else
		rc = false;

	return rc;
}

void ircBuffer::addBytes(char* bytes, size_t max_size)
	
{
	std::string temp_buffer(bytes);
	std::string temp = _buffer + temp_buffer;

	size_t split = temp.rfind("\r\n");

	//if we don't have a full message
	if(split == std::string::npos)
	{
		split = 0;
	}

	//find the start of the fragment message
	else
	{
		split += 2;

	}


	ircLog::instance()->logf(FILENAME, "temp size: %d\tsplit: %d", temp.size(), split);

	//if... something weird, not sure what is happening but it makes things work
	if(split > temp.size())
	{
		_buffer = "";
	}

	//push the message fragment into buffer
	else
	{
		_buffer = temp.substr(split);
		temp = temp.substr(0, split);

		ircLog::instance()->logf(FILENAME, "Leftovers: %s", _buffer.c_str());
		ircLog::instance()->logf(FILENAME, "temp: %s", temp.substr(0, split - 2).c_str());
	}

	//parse the complete messages into individual messages
	while(temp.find("\r\n") != std::string::npos)
	{
		std::string msg = temp.substr(0, temp.find("\r\n"));
		temp = temp.substr(temp.find("\r\n") + 2);
		_msgs.push_back(msg);
	}
}
