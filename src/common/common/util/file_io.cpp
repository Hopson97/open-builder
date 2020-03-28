#include "file_io.h"

#include <fstream>
#include <iostream>

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
