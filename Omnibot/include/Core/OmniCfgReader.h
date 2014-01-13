#ifndef _OMNI_CFG_READER_H_
#define _OMNI_CFG_READER_H

#include <string>
#include <utility>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

class OmniCfgReader
{

	public:
		int open(std::string);
		void close();
		bool getBool(std::string section, std::string entry, bool dflt);
		std::string getString(std::string section, std::string entry, std::string dflt);
		int getInt(std::string section, std::string entry, int dflt);
		double getDouble(std::string section, std::string entry, double dflt);

		bool getNextSection(std::string& section);
		bool getNextEntry(std::string& entry);

	private:
		
		std::fstream _file;

		typedef std::map<std::string, std::string> EntryMap;
		typedef std::pair<std::string, std::string> EntryPair;
		typedef std::vector<std::string> EntryVector;

		struct sectionData
		{
			EntryMap entries;
			EntryVector order;
		};


		typedef std::pair<std::string, sectionData > SectionPair;
		typedef std::map<std::string, sectionData> SectionMap;
		typedef std::pair<std::string, EntryVector > OrderEntryPair;

		SectionMap  _sections;
		std::vector<std::string> _sectionsInOrder;

		unsigned int _nextEntry;
		unsigned int _nextSection;

		int processFile();
		void addSection(std::string);
		void addEntry(std::string section, std::string entry, std::string value);
};

#endif
