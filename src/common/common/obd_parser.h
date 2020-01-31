#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct DataBlock {
    std::string type;
    std::unordered_map<std::string, std::string> data;
};

std::vector<DataBlock> getObdData(const std::string& filename);
std::vector<DataBlock> getObdDataRaw(const std::string& obd);