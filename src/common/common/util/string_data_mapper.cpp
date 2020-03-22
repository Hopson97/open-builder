#include "string_data_mapper.h"

#include <sstream>

namespace {

    std::vector<std::string> split(const std::string input, char delim)
    {
        std::vector<std::string> strings;
        std::stringstream stream(input);
        std::string line;
        while (std::getline(stream, line, delim)) {
            strings.push_back(line);
        }
        return strings;
    }
} // namespace

void StringDataMapper::parse(const std::string& data)
{
    auto pairs = split(data, ';');
    for (const auto& pair : pairs) {
        auto keyValue = split(pair, '=');
        m_parsedData.emplace(keyValue[0], keyValue[1]);
    }
}

std::string StringDataMapper::findValue(const std::string& key)
{
    return m_parsedData[key];
}
