#include<curl/curl.h>
#include<curl/types.h>
#include<curl/easy.h>

#include<cstdlib>
#include<cstring>

#include "bashbot.h"
#include "Threads/pthread_Thread.h"

#ifdef DYNAMIC_LOADING
extern "C" OmniPlugin* CreatePlugin()
{
	return static_cast<OmniPlugin*> ( new bashbot());
}
#endif

const std::string bashbot::baseURL = "www.bash.org";

void bashbot::onMessage(ircMessage& msg)
{
	std::string text = msg.message();

	std::cout << "bashbot: message is: " << text << std::endl;

	std::string cmd = text.substr(0,5);

	if(!cmd.compare("!bash") || !cmd.compare("!Bash") || !cmd.compare("!BASH"))
	{
		if(text.size() == 5)
		{
			bashbot::bashQuote* q = nextBash();
			if(!msg.isPrivateMsg())
			{
				printQuote(q, msg.channel());
				delete q;
			}
		}
		else
		{
			std::string arg = text.substr(6);
			int i  = ::atoi(arg.c_str());
			std::cout << "bashbot: the bash arg was: " << arg << " i = " << i << std::endl;
			if(i == 0)
			{
				if(!msg.isPrivateMsg())
				{
					utils->sendMessage(msg.channel(), search(arg));
				}
			}
			else
			{
				bashbot::bashQuote* q = bashNum(arg);
				if(!msg.isPrivateMsg())
				{
					printQuote(q, msg.channel());
					delete q;
				}
			}	
		}
	}
}

void bashbot::onOmniCommConnect(OmniCommChannel* channel){}

bool bashbot::init(PluginUtils* u)
{

	//assign utils
	utils = u;

	//init html escape map
	initHtmlMap();

	//init curl
	curl_global_init( CURL_GLOBAL_ALL );

	//set up cache
	cache = new std::deque<bashQuote*>();
	std::cout << "bashbot: size of cache in init: " << cache->size() << std::endl;
	cache->clear();
	std::cout << "bashbot: size of cache in init after clear: " << cache->size() << std::endl;
	cacheMutex = OmniMutex::create();

	//setup cache thread
	cachebuilder* cb = new cachebuilder(this);
	cacheThread = dynamic_cast<OmniThread*> (new pthread_Thread()); 
	cacheThread->init();
	cacheThread->addTask(cb, OmniThreadedClass::MODE_A);
	cacheThread->start();

	return true;
}

void bashbot::wrapUp()
{	
	delete cacheThread;
}

std::string bashbot::name()
{
	std::string retval= "bashbot";
	return retval;
}

void bashbot::initHtmlMap()
{
        entities["&lt;"] = "<";
        entities["&gt;"] = ">";
        entities["&amp;"] = "&";
        entities["&quot;"] = "\"";
        entities["&agrave;"] = "Ã ";
        entities["&Agrave;"] = "Ã€";
        entities["&acirc;"] = "Ã¢";
        entities["&auml;"] = "Ã¤";
        entities["&Auml;"] = "Ã„";
        entities["&Acirc;"] = "Ã‚";
        entities["&aring;"] = "Ã¥";
        entities["&Aring;"] = "Ã…";
        entities["&aelig;"] = "Ã¦";
        entities["&AElig;"] = "Ã†";
        entities["&ccedil;"] = "Ã§";
        entities["&Ccedil;"] = "Ã‡";
        entities["&eacute;"] = "Ã©";
        entities["&Eacute;"] = "Ã‰";
        entities["&egrave;"] = "Ã¨";
        entities["&Egrave;"] = "Ãˆ";
        entities["&ecirc;"] = "Ãª";
        entities["&Ecirc;"] = "ÃŠ";
        entities["&euml;"] = "Ã«";
        entities["&Euml;"] = "Ã‹";
        entities["&iuml;"] = "Ã¯";
        entities["&Iuml;"] = "Ã�";
        entities["&ocirc;"] = "Ã´";
        entities["&Ocirc;"] = "Ã”";
        entities["&ouml;"] = "Ã¶";
        entities["&Ouml;"] = "Ã–";
        entities["&oslash;"] = "Ã¸";
        entities["&Oslash;"] = "Ã˜";
        entities["&szlig;"] = "ÃŸ";
        entities["&ugrave;"] = "Ã¹";
        entities["&Ugrave;"] = "Ã™";
        entities["&ucirc;"] = "Ã»";
        entities["&Ucirc;"] = "Ã›";
        entities["&uuml;"] = "Ã¼";
        entities["&Uuml;"] = "Ãœ";
        entities["&nbsp;"] = " ";
        entities["&copy;"] = "\u00a9";
        entities["&reg;"] = "\u00ae";
        entities["&euro;"] = "\u20a0";

}

bool bashbot::refreshCache()
{

	std::cout << "bashbot: refreshing cache" << std::endl;
	bashBuffer* buffer = webget(baseURL + "/?random");

	//dump html doc into string
	std::string stuff(buffer->begin(), buffer->end());

	//dump string into string stream;
	std::stringstream html(stuff);

//	std::cout << "this is teh massive html: " << html.str();

	delete buffer; 

	std::cout << "bashbot: parsing quotes" << std::endl;
	
	while(!html.eof())
	{
		std::cout << "bashbot: parsed bash "<< cache->size() << std::endl;
		cacheMutex->lock();
		cache->push_back(parseQuote(&html));
		cacheMutex->unlock();
	}

	std::cout << "bashbot: cache refreshed" << std::endl;
	std::cout << "bashbot: cache size now " << cache->size() << std::endl;

	return true;
}

std::string bashbot::search(std::string searchString)
{
	std::cout << "bashbot: serch url: " << baseURL << "/?search=" << searchString << "&sort=0&show=25" << std::endl;

	//use libcurl to grap the web page
	std::cout << "bashbot: search: fetching the page" << std::endl;

	bashBuffer* buffer = webget(baseURL + "/?search=" 
			+ searchString + "&sort=0&show=25");

	//lib curl gave us the buffer, now lets dump it into a string
	std::cout << "bashbot: search: putting buffer contents into a string" << std::endl;

	std::string stuff(buffer->begin(), buffer->end());

	//dump string into string stream;
	std::cout << "bashbot: search: building a string stream" << std::endl;

	std::stringstream html(stuff);

	//strip the numbers out of the search
	std::cout << "bashbot: search: stripping the numbers" << std::endl;

	std::vector<std::string> nums = getBashNums(html);

	//build the string
	std::cout << "bashbot: search: building the bash string" << std::endl;

	std::string line = "| BASH |: "; 
	for(unsigned int i = 0; i < nums.size(); ++i )
	{
		line += nums[i] + " ";
	}

	//delete the bash buffer now that we're done with it
	delete buffer;

	//return the string of bash numbers
	std::cout << "bashbot: search: returned" << std::endl;
	return line;
}
bashbot::bashQuote* bashbot::nextBash()
{
	cacheMutex->lock();
	bashbot::bashQuote* q = cache->front();
	cache->pop_front();
	cacheMutex->unlock();
	
	if(cache->size() < CACHE_MIN)
	{	
		std::cout << "bashbot::nextBash: refresshing bash cache" <<std::endl;
		cachebuilder* cb = new cachebuilder(this);
		cacheThread->addTask(cb, OmniThreadedClass::MODE_A);
	}

	return q;
}
bashbot::bashQuote* bashbot::bashNum(std::string number)
{
	std::cout << "bashbot: bashnum url: " << baseURL << "/?quote=" << number << std::endl;

	bashBuffer* buffer = webget(baseURL + "/?quote=" + number);

	std::string stuff(buffer->begin(), buffer->end());

	//dump string into string stream;
	std::stringstream html(stuff);
	
	std::cout << "bashbot: bashnum is parsing the quote" << std::endl;
	bashbot::bashQuote* quote = parseQuote(&html);

	delete buffer;

	return quote;
}
bashbot::bashBuffer* bashbot::webget(std::string url)
{

	CURL* curlHandle = curl_easy_init();
	bashBuffer* buffer = new bashBuffer;

	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeMemCallback);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void*) buffer);

	curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());

	CURLcode result = curl_easy_perform( curlHandle );
	
	curl_easy_cleanup( curlHandle);
	
	return buffer;

}
void bashbot::printQuote(bashbot::bashQuote* quote, std::string channel)	
{
	//this has happend atleast once, lets gaurd against it and se if it happens again
	if(quote == NULL)
	{
		std::cout << "bashbot::printQuote: quote struct points to null" << std::endl;
		std::cout << "bashbot::printQuote: qoute cache has " << cache->size()<<" quotes" <<std::endl;
		utils->sendMessage(channel, "Error, try again");
		return;
	}


	std::cout << "bashbot: printqoute quote length is: " << quote->lines.size() << std::endl;
	std::string prefix = "| BASH " + quote->num + " |: ";

	for(size_t i = 0; i < quote->lines.size(); ++i)
	{
		std::string line = prefix + quote->lines[i];
		utils->sendMessage(channel, line);
		::usleep(SPAM_INTV);
	}
}
bashbot::bashQuote* bashbot::parseQuote(std::stringstream* src)
{
	//std::cout << src->str() << std::endl << "there it was again" << std::endl;
///	std::cout << "bashbot: allocating quote" << std::endl;
	bashbot::bashQuote* quote = new bashbot::bashQuote();
	while(!src->eof())
	{

		std::string line;
		std::getline(*src, line, '\n');
		if(src->bad()){};
			//try to fail gracefully

		if(line.find("p class=\"qt\"") != std::string::npos)
		{
			std::cout << "bashbot: bashline: " <<line << std::endl;

			quote->num = getNum(line);
			std::string tmp = getInitialLine(line);
			std::cout << "bashbot: trimed line is: " << tmp << std::endl;
			quote->lines.push_back(tmp);
			
			if(line.find("</p>", line.size() - 4) != std::string::npos)
			{
				std::cout << "bashbot: number of lines: " << quote->lines.size() << std::endl;
				return quote;
			}
		}
		else if(quote->lines.size() > 0)
		{
			if(line.find("</p>") != std::string::npos)
			{

				std::string tmp = trimBodyLine(line);
				std::cout << "bashbot: trimed line is: " << tmp << std::endl;
				quote->lines.push_back(tmp);
				std::cout << "bashbot: number of lines: " << quote->lines.size() << std::endl;
				std::cout << "bashbot: last bashline: " <<line << std::endl;
				return quote;
			}
			//quote->lines.push_back(trimBodyLine(line));
			std::string tmp = trimBodyLine(line);
			std::cout << "bashbot: trimed line is: " << tmp << std::endl;
			quote->lines.push_back(tmp);
//			std::cout << "bashbot: bashline: " <<line << std::endl;
		}



	}

	if(quote->lines.size() < 1){
		delete quote;
		quote = NULL;
	}
	return quote;

}

std::string bashbot::getNum(std::string line)
{
	size_t loc = line.find("<b>#");
	loc += 4;
	std::string retval = line.substr(loc);
	loc = retval.find("</b>");
	retval = retval.substr(0, loc);
	
	return retval;
}

std::string bashbot::getInitialLine(std::string line)
{
	size_t loc = line.find("p class=\"qt\"");

	loc += 13;

	std::string retval = line.substr(loc);
	std::cout << "bashbot: the initial line is : " << retval << std::endl;

	return trimBodyLine(retval);
}

std::vector<std::string> bashbot::getBashNums(std::stringstream& src)
{
	std::vector<std::string> nums;
	std::cout << "bashbot::getBashNums: entering loop" << std::endl;
	while(!src.eof())
	{
		//char* buf = new char[GLBUFFERSIZE];
		//src.getline(buf, GLBUFFERSIZE);
	
		std::string line;
		std::getline(src, line, '\n');

		if(src.bad()){}
			//try to fail gracefully

		//delete buf;


		if(line.find("p class=\"qt\"") != std::string::npos)
		{
			std::cout << "bashbot::getBashNums: found a num" << std::endl;
			nums.push_back(getNum(line));
		}
	}

	std::cout << "basbot::getBashNums: finished parseing nums" << std::endl;
	return nums;
}

std::string bashbot::trimBodyLine(std::string line)
{
	std::cout << "bashbot: line to trim: " << line << std::endl;

	std::string retval;
	if(line.find("<br />") != std::string::npos)
	{
		retval = line.substr(0, line.find("<br />"));
	}

	else if(line.find("</p>") != std::string::npos)
	{
		retval = line.substr(0, line.find("</p>"));
	}
	else
	{
		retval = "whoops... lost it";
	}

	std::cout << "bashbot: trimmed line is: " << retval << std::endl;
	return unescapehtml(retval, 0);

}

std::string bashbot::unescapehtml(std::string line, int start)
{

	unsigned int i, j;
	i = line.find_first_of('&', start);
	if(i != std::string::npos)
	{
		j= line.find_first_of(';', i);
		if(j != std::string::npos && j > i)
		{
			std::map<std::string, std::string>::iterator iter;

			std::string posEntity = line.substr(i, j + 1 - i);

			std::cout << "bashbot: found possible entity: " << posEntity << std::endl;

			iter =  entities.find(posEntity);
			if(iter != entities.end())
			{
				std::string entity = (*iter).second;
				line = line.substr(0,i) + entity + line.substr(j+1);

				std::cout << "bashbot: found the and replaced the entitty." <<std ::endl;
				return unescapehtml(line, start + 1);
			}
			else
			{
				std::cout << "bashbot: couldn't find a match in the table" << std::endl;
			}
		}

	}
	return line;
}

size_t bashbot::writeMemCallback(void *ptr, size_t size, size_t nmemb, void *data)
{

	std::cout << "bashbot: got some from cURL" << std::endl;
	size_t realsize = size * nmemb;

	bashBuffer* buffer = (bashBuffer*) data;

	char* buf = (char*) ptr;

	std::vector<char> v(buf, buf + realsize);

	buffer->insert(buffer->end(), v.begin(), v.end());
	
	return realsize;	
}
bashbot::cachebuilder::cachebuilder(bashbot* b):bot(b){}

void bashbot::cachebuilder::runA()
{
	bot->refreshCache();

	delete this;
}
void bashbot::cachebuilder::runB(){}
void bashbot::cachebuilder::runC(){}
void bashbot::cachebuilder::runD(){}
