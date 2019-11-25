#pragma once

#include <SFML/System/Vector3.hpp>
#include <glm/glm.hpp>
#include <ostream>
#include <vector>

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
