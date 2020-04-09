#include "util.h"
#include <algorithm>
#include <ctime>
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

RandomNumberGenerator::RandomNumberGenerator(int seed)
{
    setSeed(seed);
}

void RandomNumberGenerator::setSeed(int seed)
{
    m_rng.seed(seed);
}

int RandomNumberGenerator::nextInt(int low, int high)
{
    std::uniform_int_distribution<int> dist(low, high);
    return dist(m_rng);
}

float RandomNumberGenerator::nextFloat(float low, float high)
{
    std::uniform_real_distribution<float> dist(low, high);
    return dist(m_rng);
}

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

std::string loadFileContents(const std::string_view path)
{
    std::ifstream inFile(path.data(),
                         std::ios::in | std::ios::binary |
                             std::ios::ate); // Open the file to read in binary mode,
                                             // immediately seek to the end of the file.
    if (!inFile.is_open()) {
        std::cout << "Could not open file " << path << std::endl;
        return "";
    }
    std::string fileContents(inFile.tellg(), 0);
    inFile.seekg(0, inFile.beg);
    inFile.read(fileContents.data(), fileContents.size());
    return fileContents;
}
