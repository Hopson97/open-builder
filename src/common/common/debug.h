#pragma once

#include <SFML/System/Vector3.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <mutex>
#include <ostream>
#include <vector>

//#define DEBUG_MODE_

#ifdef DEBUG_MODE_

#define LOG(title, var)                                                        \
    std::cout << __FILE__ << ":" << __LINE__ << " - " << title << " " << var       \
              << std::endl;

#define LOG(item)                                                              \
    std::cout << __FILE__ << ":" << __LINE__ << " - " << item << std::endl;
#else

#define LOG(title, var)
#define LOG()

#endif

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &data)
{
    for (const auto &entry : data) {
        stream << entry << '\n';
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vec);
std::ostream &operator<<(std::ostream &stream, const glm::vec2 &vec);

std::ostream &operator<<(std::ostream &stream, const sf::Vector3i &vec);
// std::ostream &operator<<(std::ostream &stream, const ChunkPosition &vec);
