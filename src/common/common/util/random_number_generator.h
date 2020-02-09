#pragma once

#include <random>

class RandomNumberGenerator {
  public:
    RandomNumberGenerator();

    void setSeed(int seed);

    int nextInt(int low, int high);
    float nextFloat(float low, float high);

  private:
    std::minstd_rand m_rng;
};