#pragma once

#include <string>
#include <map>

/// <summary>Parser for .ini files.</summary>
class IniParser
{
public:
	/// <summary>Constructor taking the name of the .ini file to parse.</summary>
	/// <param name="s_fileName">Name of the .ini file to parse.</param>
	IniParser(const std::string& s_fileName); 

	/// <summary>Releases all dynamic memory to the OS.</summary>
	~IniParser(void);		  // destructor

	// Public Methods:
	/// <summary>Getter for the value of a key in the .ini file.</summary>
	/// <param name="s_group">Name of the group containing the key.</param>
	/// <param name="s_key">Name of the key</param>
	/// <returns>The value of the key, if the key exists.</returns>
	/// <exception cref="invalid_argument">Thrown if the group or key in arg 1 or 2 do not exist.</exception>
	std::string operator()(const std::string& s_group, const std::string& s_key);

private:
	// Class Fields:
	/// <summary>Stores all groups as a map with key being the group name, and all keys in a group as key/value pairs.</summary>
	/// <remarks>group => map[key,value] e.g.: { (Configuration, {(IP,x.x.x.x), (port, 1234), ...} ), (Misc, {(n, 4), (k, 19), ...} ), ...}</remarks>
	std::map<std::string, std::map<std::string, std::string>*>* values; 

	// Private Methods:
	/// <summary>Parses the given file and extracts all groups and key pairs from the file.</summary>
	/// <param name="s_file">The file to parse.</param>
	void parseFile(const std::string& s_file);
};