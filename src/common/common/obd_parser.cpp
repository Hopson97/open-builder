#include "obd_parser.h"

#include "file_io.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
void cleanString(std::string &value)
{
    value.erase(value.begin(),
                std::find_if(value.begin(), value.end(),
                             std::bind1st(std::not_equal_to<char>(), ' ')));
    value.erase(std::find_if(value.rbegin(), value.rend(),
                             std::bind1st(std::not_equal_to<char>(), ' '))
                    .base(),
                value.end());
}
} // namespace

std::vector<DataBlock> getObdDataRaw(const std::string &obd)
{
    std::vector<DataBlock> data;
    std::istringstream stream(obd);
    std::string line;

    while (std::getline(stream, line)) {
        cleanString(line);
        if (line.empty()) {
            continue;
        }
        DataBlock block;
        block.type = line;
        while (stream >> line) {
            if (line == "end") {
                break;
            }
            else {
                std::string value;
                stream >> value;
                block.data.emplace(line, value);
            }
        }
        data.push_back(std::move(block));
    }
    return data;
}

std::vector<DataBlock> getObdData(std::string &filename)
{
    return getObdDataRaw(loadFileContents(filename));
}