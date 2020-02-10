#include "obd_parser.h"

#include "file_io.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

std::unordered_map<std::string, std::string> parseObdData(const std::string& obdData)
{
	std::istringstream contents(obdData);
	std::string key;
	std::string value;
	std::unordered_map<std::string, std::string> data;

	while (contents >> key) {
		contents >> value;
		data.emplace(key, value);
	}

	return data;
}

std::string serializeObdData(const std::unordered_map<std::string, std::string>& obdData)
{
	std::ostringstream output;
	for (auto const& pair : obdData) {
		output << pair.first << " " << pair.second << "\n";
	}
	return output.str();
}
