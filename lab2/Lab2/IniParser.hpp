#pragma once

#include <string>
#include <map>


class IniParser
{
public:
	// Constructor:
	IniParser(const std::string&); // constructor taking the path to a *.ini file

	// Destructor:
	~IniParser(void);		  // destructor

	// Public Methods:
	std::string operator()(const std::string& group, const std::string& key); // retrieves the key in group if it exists

private:
	// Class Fields:                                                                                         group name     key  value
	std::map<std::string, std::map<std::string, std::string>*>* values; // group => map<key,value> e.g.: { (Configuration, {(IP,x.x.x.x), (port, 1234), ...} ), (Misc, {(n, 4), (k, 19), ...} ), ...}

	// Private Methods:
	void parseFile(const std::string&); // parses the given *.ini file into values
};