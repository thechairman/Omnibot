#ifndef _IRC_BUFFER_H_
#define _IRC_BUFFER_H_

#include <string>
#include <deque>

class ircBuffer{

	public:
		std::string nextMessage();
		bool hasNextMessage();
		void addBytes(char*, size_t);

	private:
		
		void parse();
		std::string _buffer;
		std::deque<std::string> _msgs;

};

#endif
