#include "random_number_generator.h"

#include <ctime>

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
