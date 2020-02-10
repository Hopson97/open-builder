#pragma once

#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, std::string> parseObdData(const std::string& filename);
std::string serializeObdData(const std::unordered_map<std::string, std::string>& obdData);
