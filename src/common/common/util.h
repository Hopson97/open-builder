#pragma once

#include <random>
#include <string>
#include <string_view>
#include <unordered_map>

class RandomNumberGenerator {
  public:
    RandomNumberGenerator(int seed = 0);

    void setSeed(int seed);

    int nextInt(int low, int high);
    float nextFloat(float low, float high);

  private:
    std::minstd_rand m_rng;
};

std::unordered_map<std::string, std::string> getObdData(const std::string& filename);
std::unordered_map<std::string, std::string> getObdDataRaw(const std::string& obd);

std::string loadFileContents(const std::string_view path);
