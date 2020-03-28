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

std::unordered_map<std::string, std::string> getObdDataRaw(const std::string& obd)
{
    std::unordered_map<std::string, std::string> data;
    std::istringstream stream(obd);
    std::string line;
    std::string value;

    while (stream >> line) {
        if (line.empty()) {
            continue;
        }
        stream >> value;
        data.emplace(line, value);
    }

    return data;
}

std::unordered_map<std::string, std::string> getObdData(const std::string& filename)
{
    return getObdDataRaw(loadFileContents(filename));
}