#include "wordsbot.h"
#include <deque>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cctype>
#include "ircLog.h"


#define MYFILE 	"wordsbot.cpp"

#ifdef DYNAMIC_LOADING
	extern "C" OmniPlugin* CreatePlugin()
	{
		return static_cast<OmniPlugin*> (new wordsbot());
	}
#endif

const std::string wordsbot::WORDS_FNAME = "word_map.txt";
const std::string wordsbot::LINES[NUM_LINES][MAX_LINE_LEN] =
{
	{ "Someone said ", " is "},
	{ "I guess ", " is "},
	{ "", " is probably "},
	{ "I'm sure ", " is "},
	{ "It's been said that ", " is "},
	{ "Well, ", " is " },
	{ "I heard that ", " is " },
	{ "Hmm, I think ", " is " }
};


//Some one said X Y
//I guess x is y
//x is probaly y
//I'm sure x is y
//Its been said that x is y
//well, x is y
//I heard that x is y
//hmm, i think x is y

bool wordsbot::proc()
{
	bool proc = false;
	srand(time(NULL));

	int result = rand() % 10;
	ircLog::instance()->logf(MYFILE, " - proc(): result was %d", result);
	if(result < 5)
	{
		proc = true;
	}

	return true;

}

void wordsbot::onMessage(ircMessage& msg)
{
	std::string normalized_msg = msg.message();
	std::transform(normalized_msg.begin(), normalized_msg.end(), normalized_msg.begin(), ::tolower);
	std::string output = "";

	ircLog::instance()->logf(MYFILE, " - onMessage() Normalized String: %s", normalized_msg.c_str());

	if(std::string::npos != normalized_msg.rfind("no", 3))
	{
		ircLog::instance()->logf(MYFILE, " - onMessage() found \"no\"");
		if(addString(msg.message()))
		{
			output = "Ok, " + msg.user().nick(); 
			ircLog::instance()->logf(MYFILE, " - onMessage() added string returning %s", output.c_str());
		}
	}
	else
	{
		if(!addString(msg.message()))
		{

			ircLog::instance()->logf(MYFILE, " - onMessage() didn't add the string");
			//search for string in table
			std::pair<std::string, std::string>* strs = NULL;
			strs = searchString(msg.message());

			if(strs != NULL)
			{
				output = getString(strs->first, strs->second);
				ircLog::instance()->logf(MYFILE, " - onMessage() found match, returning: %s", output.c_str());
				delete strs;
			}
		}
	}

	//if the content of ouput has changed print it to the channel.
	if(output.compare(""))
	{
		ircLog::instance()->logf(MYFILE, " - onMessage() found match, returning output");
		utils->sendMessage(msg.channel(), output);
	}
}

void wordsbot::onOmniCommConnect(OmniCommChannel* channel){}

bool wordsbot::init(PluginUtils* utils_){
	utils = utils_;
	loadWords();
	return true;
}

void wordsbot::wrapUp(){
	saveWords();
}

std::string wordsbot::name(){
	std::string name = "wordsbot";
	return name;
}

std::string wordsbot::getString(const std::string& x, const std::string& y)
{
	int string;

	std::string retval;

	//get rand value for string.
	srand(time(NULL));
	string = rand() % (NUM_LINES - 1);
	ircLog::instance()->logf(MYFILE, " - getString() random value: %d", string);

	retval = LINES[string][FIRST] + x + LINES[string][SECOND] + y;
	ircLog::instance()->logf(MYFILE, " - getString() returing string %s", retval.c_str());

	return retval;

}

bool wordsbot::addString(const std::string& str)
{

	std::stringstream tokenizer(str);
	std::string token;
	std::deque<std::string> before, after;
	std::string x, y;
	bool found_is = false;
	bool found_no = false;

	std::string normalized_str = str;
	std::transform(normalized_str.begin(), normalized_str.end(), normalized_str.begin(), ::tolower);
	ircLog::instance()->logf(MYFILE, " - addString() normalized string: %s", normalized_str.c_str()); 
	if(std::string::npos != normalized_str.rfind("no", 3))
	{
		ircLog::instance()->logf(MYFILE, " - addString() found \"no\"");
		found_no = true;
	}

	while(getline(tokenizer, token, ' '))
	{
		if(!token.compare("is"))
		{
			ircLog::instance()->logf(MYFILE, " - addString() found \"is\"");
			found_is = true;
		}
		else if(found_is)
		{
			after.push_back(token);
		}
		else
		{
			before.push_back(token);
		}
	}

	//if its a no, x is y string we want everything between no,
	//and is for x, else we just want the word before.
	

	if(found_is)
	{
		ircLog::instance()->logf(MYFILE, " - addString() found_is is true");
	}
	else
	{
		ircLog::instance()->logf(MYFILE, " - addString() found_is is false");
	}
	if(found_no)
	{
		ircLog::instance()->logf(MYFILE, " - addString() found_no is true");
	}
	else
	{
		ircLog::instance()->logf(MYFILE, " - addString() found_no is false");
	}

	if(found_is && found_no)
	{
		ircLog::instance()->logf(MYFILE, " - addString() found both");
		size_t is = str.find("is");
		//start from the end of "no, "
		size_t after_no = str.find(" ") + 1;
		if(is - after_no > 0)
		{
			//the minus one is to not get the space before is
			x = str.substr(after_no, is - after_no - 1 );
		}
		else
		{
			found_is = false;
		}
	}
	else if(found_is)
	{
		ircLog::instance()->logf(MYFILE, " - addString() find only is");
		if(before.size() > 0)
		{	
			x = (*before.rbegin());
		}
		else
		{
			found_is = false;
		}
	}
	
	ircLog::instance()->logf(MYFILE, " - addString() x is %s", x.c_str());
	//handle the y which is the same for either case
	if(found_is)
	{
		y = "";
		std::deque<std::string>::iterator iter;
		for(iter = after.begin(); iter != after.end(); iter++)
		{
			y += (*iter) + " ";
		}

		y.erase(y.size() - 1, 1);
		ircLog::instance()->logf(MYFILE, " - addString() y is %s", y.c_str());
		std::pair<std::string, std::string> apair;
		apair = std::make_pair<std::string, std::string>(x,y);
		_words.insert(apair);
		saveWords();
	}

	return found_is;
}

std::pair<std::string, std::string>* wordsbot::searchString(const std::string& str)
{
	size_t question_mark = str.find("?");
	size_t ex_point = str.find("!");
	bool random = true;
	std::map<std::string, std::string>::iterator iter;
	std::pair<std::string, std::string>* the_pair = NULL;

	for(iter = _words.begin(); iter != _words.end(); iter++)
	{
		ircLog::instance()->logf(MYFILE, " - searchString() checking for: %s", iter->first.c_str()); 

		size_t found = std::string::npos;
		if(question_mark != std::string::npos)
		{

			ircLog::instance()->logf(MYFILE, " - searchString() found a question mark"); 
			found = str.rfind((iter->first), question_mark);
			random = false;
		}
		else if(ex_point != std::string::npos)
		{
			ircLog::instance()->logf(MYFILE, " - searchString() found exclamation"); 
			found = str.rfind((iter->first), ex_point);
			random = false;
		}
		else
		{
			found = str.find((iter->first));
			ircLog::instance()->logf(MYFILE, " - searchString() co: %s", iter->first.c_str()); 
		}

		//compensate for finding a word inside another word
		//ircLog::instance()->logf(MYFILE, " - searcString(): finished searching
		if(found != std::string::npos && found > 0 && str[found -1] != ' ')
		{
			found = std::string::npos;
		}
	


		if( found != std::string::npos &&
				found + (iter->first).size() < str.size() && 
				str[found + (iter->first).size()] != ' ' &&
				str[found + (iter->first).size()] != '?' &&
				str[found + (iter->first).size()] != '!')
		{
			found = std::string::npos;
		}


		if(found != std::string::npos)
		{
			break;
		}
	}

	if(iter != _words.end())
	{
		
		ircLog::instance()->logf(MYFILE, " - searchString() returning pair: %s ---> %s", 
				iter->first.c_str(), iter->second.c_str()); 
		if(random)
		{
			if(proc())
			{
				the_pair = new std::pair<std::string, std::string>((*iter));
			}

		}
		else
		{
			the_pair = new std::pair<std::string, std::string>((*iter));
		}
	}

	return the_pair;
}
void wordsbot::saveWords()
{
	std::ofstream file;

	std::map<std::string, std::string>::iterator iter;
	file.open(WORDS_FNAME.c_str());

	for(iter = _words.begin(); iter != _words.end(); iter++)
	{
		ircLog::instance()->logf(MYFILE, " - saveWords() %s\t--->\t%s", iter->first.c_str(), iter->second.c_str());
		file << iter->first << "\t--->\t" << iter->second <<"\n" ;
	}


	file.close();
}
void wordsbot::loadWords()
{
	std::ifstream file;
	std::string file_line;
	std::stringstream tokenizer;
	std::string token;

	file.open(WORDS_FNAME.c_str());

	while(std::getline(file, file_line, '\n'))
	{
		/*
		if(std::string::npos != file_line.find('\t'))
		{
			ircLog::instance()->logf(MYFILE, " - loadWords(): file line has a tab char");
		}

		if(std::string::npos != file_line.find('\n'))
		{
			ircLog::instance()->logf(MYFILE, " - loadWords(): file line has a new line");
		}


		std::string x, y;
		tokenizer.str(file_line);
		ircLog::instance()->logf(MYFILE, " - loadWords() file line %s", file_line.c_str());
		ircLog::instance()->logf(MYFILE, " - loadWords() tokenizer contents %s", tokenizer.str().c_str());
		std::getline(tokenizer, token, '\t');
		x = token;
		ircLog::instance()->logf(MYFILE, " - loadWords() first token %s", token.c_str());
		std::getline(tokenizer, token, '\t');
		//that one is the --->
		std::getline(tokenizer, token, '\n');
		y = token;
		ircLog::instance()->logf(MYFILE, " - loadWords() second tokent %s", token.c_str());
		ircLog::instance()->logf(MYFILE, " - loadWords() maping: %s ===> %s", x.c_str(), y.c_str() );
		*/


		ircLog::instance()->logf(MYFILE, " - loadWords() file line %s", file_line.c_str());

		size_t start, count;
		std::string x, y;

		start = 0;
		count = file_line.find('\t');
		x = file_line.substr(start, count);

		start = file_line.rfind('\t') + 1;
		count = file_line.size() - start;
		y = file_line.substr(start, count);

		ircLog::instance()->logf(MYFILE, " - loadWords() maping: %s ===> %s", x.c_str(), y.c_str() );


		std::pair<std::string, std::string> p(x,y);
		_words.insert(p);
	}

	file.close();

}
