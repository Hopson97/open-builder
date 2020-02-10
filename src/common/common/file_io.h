#pragma once

#include <string>

/**
 * @brief Loads the entire content from a text file
 * @param path The path of the file to load
 * @return std::string Contents of the file
 */
std::string loadFileContents(const std::string& path);
bool fileExists(const std::string& path);
