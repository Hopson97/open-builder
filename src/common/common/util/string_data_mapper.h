#pragma once

#include <string>
#include <unordered_map>

class StringDataMapper
{
  public:
    void parse(const std::string& data);

    // Lua API
    std::string getString(const std::string& key) const;
    int getInt(const std::string& key) const;
    float getFloat(const std::string& key) const;
    
  private:
    std::unordered_map<std::string, std::string> m_parsedData;
};