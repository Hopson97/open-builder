#pragma once

#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, std::string> parseObdData(const std::string& obdData);
std::string serializeObdData(const std::unordered_map<std::string, std::string>& obdData);

class ObdConfig
{
		std::string m_filePath;
	public:
		ObdConfig(const std::string& filePath); // implicitly read config file
		~ObdConfig(); // write back to file when the config goes out of scope
	protected:
		std::unordered_map<std::string, std::string> m_keyvalues;
};
