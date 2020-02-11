#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>

#include "obd_config.h"

// member definitions for ObdConfig
ObdConfig::ObdConfig(const std::string& filePath)
	: m_filePath(filePath)
{
	if (std::filesystem::exists(filePath)) {
		m_filePath = filePath;
		std::ifstream file(m_filePath);
		if (!file.is_open()) {
			std::cerr << "Could not open file " << m_filePath << std::endl;
			return;
		}
		std::stringstream stream;
		stream << file.rdbuf();

		m_keyvalues = parseObdData(stream.str());
	}
}

ObdConfig::~ObdConfig()
{
	std::ofstream file(m_filePath);
    if (!file.is_open()) {
        std::cerr << "Could not write to " << m_filePath << std::endl;
		return;
    }
	file << serializeObdData(m_keyvalues);
}

// helper functions
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
