#include "obd_parser.h"

#include "file_io.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
/**
    Removes trailing/ leading whitespace from a string
*/
std::string cleanString(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}
} // namespace

std::vector<DataBlock> getObdDataRaw(const std::string& obd)
{
    std::vector<DataBlock> data;
    std::istringstream stream(obd);
    std::string line;

    while (std::getline(stream, line)) {
        line = cleanString(line);
        if (line.empty()) {
            continue;
        }
        DataBlock block;
        block.type = line;
        while (stream >> line) {
            if (line.empty()) {
                continue;
            }
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

std::vector<DataBlock> getObdData(const std::string& filename)
{
    return getObdDataRaw(loadFileContents(filename));
}