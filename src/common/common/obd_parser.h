#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct DataVoxel {
    std::string type;
    std::unordered_map<std::string, std::string> data;
};

std::vector<DataVoxel> getObdData(const std::string& filename);
std::vector<DataVoxel> getObdDataRaw(const std::string& obd);