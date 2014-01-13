#include <sstream>
#include <algorithm>

#include "OmniCfgReader.h"

int OmniCfgReader::open(std::string fileName)
{
	_file.open(fileName.c_str(), std::fstream::in);
	_nextSection = 0;
	_nextEntry = 0;

	int retval = processFile();
	std::cout << "processFile returned: " << retval << std::endl;
	return retval; 

}

void OmniCfgReader::close()
{
	_file.close();
}

bool OmniCfgReader::getBool(std::string section, std::string entry, bool dflt)
{
	bool retval = dflt;
	SectionMap::iterator iter = _sections.find(section);
	if(iter != _sections.end())
	{
		EntryMap::iterator eiter = iter->second.entries.find(entry);
		if(eiter != iter->second.entries.end())
		{
			std::string temp = eiter->second;
			if(!temp.compare("true") || !temp.compare("1"))
			{
				retval = true;
			}
			else if(!temp.compare("false") || !temp.compare("0"))
			{
				retval = false;
			}
			else
			{
				std::cerr << "ERROR invalid value: " << temp << std::endl;
			}
		}
	
	}
	return retval;

}
std::string OmniCfgReader::getString(std::string section, std::string entry, std::string dflt)
{
	std::string retval = dflt;
	SectionMap::iterator iter = _sections.find(section);
	if(iter != _sections.end())
	{
		EntryMap::iterator eiter = iter->second.entries.find(entry);
		if(eiter != iter->second.entries.end())
		{
			retval = eiter->second;
		}
	}
	return retval;
}
int OmniCfgReader::getInt(std::string section, std::string entry, int dflt)
{
	int retval = dflt;
	SectionMap::iterator iter = _sections.find(section);
	if(iter != _sections.end())
	{
		EntryMap::iterator eiter = iter->second.entries.find(entry);
		if(eiter != iter->second.entries.end())
		{
			std::stringstream ss(eiter->second);
			ss >> retval;
		}
	}
	return retval;
}
double OmniCfgReader::getDouble(std::string section, std::string entry, double dflt)
{
	double retval = dflt;
	SectionMap::iterator iter = _sections.find(section);
	if(iter != _sections.end())
	{
		EntryMap::iterator eiter = iter->second.entries.find(entry);
		if(eiter != iter->second.entries.end())
		{
			std::stringstream ss(eiter->second);
			ss >> retval;
		}
	}
	return retval;
}
int OmniCfgReader::processFile()
{
	std::string line;
	std::string section = "";
	int lineNumber = 0;
	while(std::getline(_file,line ))
	{
		++lineNumber;
		
		if(!line.compare("") || !line.compare("\n"))
			continue;

		//skip commented lines
		if(line[0] == '#')
		{
			continue;
		}

		//catch section delimeter lines
		if(!line.substr(0,2).compare( "[ "))
		{
			size_t ndx = line.find_first_of(']');
			if(ndx == std::string::npos)
			{
				std::cerr << "Line " << lineNumber << 
				": Parse Error: no closing ']'" << std::endl;	
			}
			else
			{
				ndx -= 2;
				section = line.substr(2, ndx - 1);
				addSection(section);
				std::cout << "Section: \"" << section << "\"" << std::endl;
			}
		}
		else
		{
			size_t entryEnd = line.find_first_of('=');
			if(!section.compare(""))
			{
				std::cerr << "Line " << lineNumber <<
				": Parse Error: No section specified" << std::endl;	
			}
			else if(entryEnd == std::string::npos)
			{
				std::cerr << "Line " << lineNumber << 
				": Parse Error: no '=' in entry line" <<
				std::endl;
			}
			else
			{
				entryEnd -=1;
				std::string entry = line.substr(0,entryEnd);
				entryEnd +=3;
				std::string value = line.substr(entryEnd);
				value.erase( std::remove(value.begin(), value.end(), '"'), value.end());
				addEntry(section, entry, value);
				std::cout << "Entry: \"" << entry << "\" = \"" << value << "\"" << std::endl;
			}
		}
	}
	for(int i = 0; i < _sectionsInOrder.size(); ++i )
	{
		std::cout << "Section: " << _sectionsInOrder[i] << std::endl;
		sectionData data = _sections[_sectionsInOrder[i]];
		EntryMap::iterator iter = data.entries.begin();
	 	for(iter = data.entries.begin(); iter != data.entries.end(); iter++)
		{
			std::cout << "Entry: " << iter->first << " = " << iter->second << std::endl;
		}

		std::cout << std::endl;
		
	}
	return 0;
}
void OmniCfgReader::addSection(std::string section)
{
	sectionData data;
	_sections.insert(SectionPair(section, data));
	_sectionsInOrder.push_back(section);
}


void OmniCfgReader::addEntry(std::string section, std::string entry, std::string value)
{
	//check section exists in _sections
	SectionMap::iterator iter = _sections.find(section);
	if(iter == _sections.end())
	{
		addSection(section);
		iter = _sections.find(section);
	}

	//check that it is in the entry we found.
	EntryMap::iterator e_iter = iter->second.entries.find(entry);
	if(e_iter == iter->second.entries.end())
	{
		//add a new value to the entry map
		iter->second.entries.insert(EntryPair(entry, value));
		iter->second.order.push_back(entry);
	}
	else
	{
		//update the value we found
		e_iter->second = value;

   		EntryVector::iterator vec_iter;
   		for(vec_iter = iter->second.order.begin();
   				vec_iter != iter->second.order.end();
   				vec_iter++)
   		{
   			if(!(*vec_iter).compare(entry))
   			{
   				iter->second.order.erase(vec_iter);
   				iter->second.order.push_back(entry);
   				break;
   			}
   		}
	}
}

bool OmniCfgReader::getNextSection(std::string& section)
{
	bool retval = false;
	if(_nextSection < _sectionsInOrder.size())
	{
		section = _sectionsInOrder[_nextSection++];
		_nextEntry = 0;
		retval = true;
	}
	else
	{
		_nextSection = 0;;
		_nextEntry = 0;
	}

	return retval;
}

bool OmniCfgReader::getNextEntry(std::string& entry)
{
	bool retval = false;
	sectionData& data =  _sections[_sectionsInOrder[_nextSection -1]];
	
	if(_nextEntry <  data.order.size())
	{
		entry = data.order[_nextEntry++];
		retval = true;
	}
	else
	{
		_nextEntry = 0;
	}

	return retval;

}
