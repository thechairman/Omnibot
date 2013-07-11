#include<curl/curl.h>
#include<curl/types.h>
#include<curl/easy.h>

#include<cstdlib>
#include<cstring>

#include "qdbbot.h"
#include "Threads/pthread_Thread.h"


#ifdef DYNAMIC_LOADING
extern "C" OmniPlugin* CreatePlugin()
{
	return static_cast<OmniPlugin*> (new qdbbot());
}
#endif

const std::string qdbbot::baseURL = "qdb.us";

void qdbbot::onMessage(ircMessage& msg)
{
	std::string text = msg.message();

	std::cout << "qdb: message is: " << text << std::endl;

	std::string cmd = text.substr(0,4);

	if(!cmd.compare("!qdb") || !cmd.compare("!qdb") || !cmd.compare("!qdb"))
	{
		if(text.size() == 4)
		{
			qdbbot::bashQuote* q = nextBash();
			if(!msg.isPrivateMsg())
			{
				printQuote(q, msg.channel());
				delete q;
			}
		}
		else
		{
			std::string arg = text.substr(5);
			int i  = ::atoi(arg.c_str());
			std::cout << "qdbbot: the bash arg was: " << arg << " i = " << i << std::endl;
			if(i == 0)
			{
				if(!msg.isPrivateMsg())
				{
					utils->sendMessage(msg.channel(), search(arg));
				}
			}
			else
			{
				qdbbot::bashQuote* q = bashNum(arg);
				if(!msg.isPrivateMsg())
				{
					printQuote(q, msg.channel());
					delete q;
				}
			}	
		}
	}
}

void qdbbot::onOmniCommConnect(OmniCommChannel* channel){}

bool qdbbot::init(PluginUtils* u)
{

	//assign utils
	utils = u;

	//init html escape map
	initHtmlMap();

	//init curl
	curl_global_init( CURL_GLOBAL_ALL );

	//set up cache
	cache = new std::deque<bashQuote*>();
	std::cout << "qdbbot: size of cache in init: " << cache->size() << std::endl;
	cache->clear();
	std::cout << "qdbbot: size of cache in init after clear: " << cache->size() << std::endl;
	cacheMutex = OmniMutex::create();

	//setup cache thread
	cachebuilder* cb = new cachebuilder(this);
	cacheThread = dynamic_cast<OmniThread*> (new pthread_Thread()); 
	cacheThread->init();
	cacheThread->addTask(cb, OmniThreadedClass::MODE_A);
	cacheThread->start();

	return true;
}

void qdbbot::wrapUp()
{	
	delete cacheThread;
}

std::string qdbbot::name()
{
	std::string retval= "qdbbot";
	return retval;
}

void qdbbot::initHtmlMap()
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

bool qdbbot::refreshCache()
{

	std::cout << "qdbbot: refreshing cache" << std::endl;
	bashBuffer* buffer = webget(baseURL + "/random");

	//dump html doc into string
	std::string stuff(buffer->begin(), buffer->end());

	//dump string into string stream;
	std::stringstream html(stuff);

//	std::cout << "this is teh massive html: " << html.str();

	delete buffer; 

	std::cout << "qdbbot: parsing quotes" << std::endl;
	
	while(!html.eof())
	{
		std::cout << "qdbbot: parsed bash "<< cache->size() << std::endl;
		cacheMutex->lock();
		cache->push_back(parseQuote(&html));
		cacheMutex->unlock();
	}

	std::cout << "qdbbot: cache refreshed" << std::endl;
	std::cout << "qdbbot: cache size now " << cache->size() << std::endl;

	return true;
}

std::string qdbbot::search(std::string searchString)
{
	std::cout << "qdbbot: serch url: " << baseURL << "/?search=" << searchString << "&sort=0&show=25" << std::endl;

	//use libcurl to grap the web page
	std::cout << "qdbbot: search: fetching the page" << std::endl;

	bashBuffer* buffer = webget(baseURL + "/search?q=" 
			+ searchString);

	//lib curl gave us the buffer, now lets dump it into a string
	std::cout << "qdbbot: search: putting buffer contents into a string" << std::endl;

	std::string stuff(buffer->begin(), buffer->end());

	//dump string into string stream;
	std::cout << "qdbbot: search: building a string stream" << std::endl;

	std::stringstream html(stuff);

	//strip the numbers out of the search
	std::cout << "qdbbot: search: stripping the numbers" << std::endl;

	std::vector<std::string> nums = getBashNums(html);

	//build the string
	std::cout << "qdbbot: search: building the bash string" << std::endl;

	std::string line = "| QDB |: "; 
	for(unsigned int i = 0; i < nums.size(); ++i )
	{
		line += nums[i] + " ";
	}

	//delete the bash buffer now that we're done with it
	delete buffer;

	//return the string of bash numbers
	std::cout << "qdbbot: search: returned" << std::endl;
	return line;
}
qdbbot::bashQuote* qdbbot::nextBash()
{
	cacheMutex->lock();
	qdbbot::bashQuote* q = cache->front();
	cache->pop_front();
	cacheMutex->unlock();
	
	if(cache->size() < CACHE_MIN)
	{	
		std::cout << "qdbbot::nextBash: refresshing bash cache" <<std::endl;
		cachebuilder* cb = new cachebuilder(this);
		cacheThread->addTask(cb, OmniThreadedClass::MODE_A);
	}

	return q;
}
qdbbot::bashQuote* qdbbot::bashNum(std::string number)
{
	std::cout << "qdbbot: bashnum url: " << baseURL << "/" << number << std::endl;

	bashBuffer* buffer = webget(baseURL + "/" + number);

	std::string stuff(buffer->begin(), buffer->end());

	//dump string into string stream;
	std::stringstream html(stuff);
	
	std::cout << "qdbbot: bashnum is parsing the quote" << std::endl;
	qdbbot::bashQuote* quote = parseQuote(&html);

	delete buffer;

	return quote;
}
qdbbot::bashBuffer* qdbbot::webget(std::string url)
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
void qdbbot::printQuote(qdbbot::bashQuote* quote, std::string channel)	
{
	//this has happend atleast once, lets gaurd against it and se if it happens again
	if(quote == NULL)
	{
		std::cout << "qdbbot::printQuote: quote struct points to null" << std::endl;
		std::cout << "qdbbot::printQuote: qoute cache has " << cache->size()<<" quotes" <<std::endl;
		utils->sendMessage(channel, "Error, try again");
		return;
	}


	std::cout << "qdbbot: printqoute quote length is: " << quote->lines.size() << std::endl;
	std::string prefix = "| QDB " + quote->num + " |: ";

	for(size_t i = 0; i < quote->lines.size(); ++i)
	{
		std::string line = prefix + quote->lines[i];
		utils->sendMessage(channel, line);
		::usleep(SPAM_INTV);
	}
}
qdbbot::bashQuote* qdbbot::parseQuote(std::stringstream* src)
{
	//std::cout << src->str() << std::endl << "there it was again" << std::endl;
///	std::cout << "qdbbot: allocating quote" << std::endl;
	qdbbot::bashQuote* quote = new qdbbot::bashQuote();
	while(!src->eof())
	{

		std::string line;
		std::getline(*src, line, '\n');
		if(src->bad()){};
			//try to fail gracefully
			//
		size_t loc = line.find("span class=qt"); 

		if(loc != std::string::npos)
		{
			std::cout << "qdbbot: bashline: " <<line << std::endl;

			quote->num = getNum(line);
			std::string tmp = getInitialLine(line);
			std::cout << "qdbbot: trimed line is: " << tmp << std::endl;
			quote->lines.push_back(tmp);
			
			if(line.find("</span>", loc) != std::string::npos)
			{
				std::cout << "qdbbot: number of lines: " << quote->lines.size() << std::endl;
				return quote;
			}
		}
		else if(quote->lines.size() > 0)
		{
			if(line.find("</span>") != std::string::npos)
			{

				std::string tmp = trimBodyLine(line);
				std::cout << "qdbbot: trimed line is: " << tmp << std::endl;
				quote->lines.push_back(tmp);
				std::cout << "qdbbot: number of lines: " << quote->lines.size() << std::endl;
				std::cout << "qdbbot: last bashline: " <<line << std::endl;
				return quote;
			}
			//quote->lines.push_back(trimBodyLine(line));
			std::string tmp = trimBodyLine(line);
			std::cout << "qdbbot: trimed line is: " << tmp << std::endl;
			quote->lines.push_back(tmp);
//			std::cout << "qdbbot: bashline: " <<line << std::endl;
		}



	}

	if(quote->lines.size() < 1){
		delete quote;
		quote = NULL;
	}
	return quote;

}

std::string qdbbot::getNum(std::string line)
{
	size_t loc = line.find(">#");
	std::string retval = line.substr(loc + 2); //start after the found characters
	loc = retval.find("</a>");
	retval = retval.substr(0, loc);
	
	return retval;
}

std::string qdbbot::getInitialLine(std::string line)
{
	//size_t loc = line.find("p class=\"qt\"");

	//loc += 13;
	//
	size_t loc = line.find("<span class=qt");
	loc = line.find (">", loc);

	std::string retval = line.substr(loc + 1);
	std::cout << "qdbbot: the initial line is : " << retval << std::endl;

	return trimBodyLine(retval);
}

std::vector<std::string> qdbbot::getBashNums(std::stringstream& src)
{
	std::vector<std::string> nums;
	std::cout << "qdbbot::getBashNums: entering loop" << std::endl;
	while(!src.eof())
	{
		//char* buf = new char[GLBUFFERSIZE];
		//src.getline(buf, GLBUFFERSIZE);
	
		std::string line;
		std::getline(src, line, '\n');

		if(src.bad()){}
			//try to fail gracefully

		//delete buf;


		if(line.find("<span class=qt") != std::string::npos)
		{
			std::cout << "qdbbot::getBashNums: found a num" << std::endl;
			nums.push_back(getNum(line));
		}
	}

	std::cout << "basbot::getBashNums: finished parseing nums" << std::endl;
	return nums;
}

std::string qdbbot::trimBodyLine(std::string line)
{
	std::cout << "qdbbot: line to trim: " << line << std::endl;

	std::string retval;
	if(line.find("<br />") != std::string::npos)
	{
		retval = line.substr(0, line.find("<br />"));
	}

	else if(line.find("</p>") != std::string::npos)
	{
		retval = line.substr(0, line.find("</p>"));
	}
	else if(line.find("</span>") != std::string::npos)
	{
		retval = line.substr(0, line.find("</span>"));
	}
	else
	{
		retval = "whoops... lost it";
	}

	std::cout << "qdbbot: trimmed line is: " << retval << std::endl;
	return unescapehtml(retval, 0);

}

std::string qdbbot::unescapehtml(std::string line, int start)
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

			std::cout << "qdbbot: found possible entity: " << posEntity << std::endl;

			iter =  entities.find(posEntity);
			if(iter != entities.end())
			{
				std::string entity = (*iter).second;
				line = line.substr(0,i) + entity + line.substr(j+1);

				std::cout << "qdbbot: found the and replaced the entitty." <<std ::endl;
				return unescapehtml(line, start + 1);
			}
			else
			{
				std::cout << "qdbbot: couldn't find a match in the table" << std::endl;
			}
		}

	}
	return line;
}

size_t qdbbot::writeMemCallback(void *ptr, size_t size, size_t nmemb, void *data)
{

	std::cout << "qdbbot: got some from cURL" << std::endl;
	size_t realsize = size * nmemb;

	bashBuffer* buffer = (bashBuffer*) data;

	char* buf = (char*) ptr;

	std::vector<char> v(buf, buf + realsize);

	buffer->insert(buffer->end(), v.begin(), v.end());
	
	return realsize;	
}
qdbbot::cachebuilder::cachebuilder(qdbbot* b):bot(b){}

void qdbbot::cachebuilder::runA()
{
	bot->refreshCache();

	delete this;
}
void qdbbot::cachebuilder::runB(){}
void qdbbot::cachebuilder::runC(){}
void qdbbot::cachebuilder::runD(){}
