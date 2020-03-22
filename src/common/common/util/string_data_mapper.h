#pragma once

#include <string>
#include <unordered_map>

class StringDataMapper
{
  public:
    void parse(const std::string& data);

    std::string findValue(const std::string& key);
    
  private:
    std::unordered_map<std::string, std::string> m_parsedData;
};