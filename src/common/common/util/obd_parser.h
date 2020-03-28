#pragma once

#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> getObdData(const std::string& filename);
std::unordered_map<std::string, std::string> getObdDataRaw(const std::string& obd);