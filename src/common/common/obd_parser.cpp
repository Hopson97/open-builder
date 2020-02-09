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
		data.insert(std::make_pair(key, value));
	}

	return data;
}
