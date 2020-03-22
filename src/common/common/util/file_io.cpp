#include "file_io.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string loadFileContents(const std::string& path)
{
    std::ifstream inFile(path);
    if (!inFile.is_open()) {
        std::cout << "Could not open file " << path << std::endl;
        return "";
    }
    std::stringstream stream;
    stream << inFile.rdbuf();
    return stream.str();
}
