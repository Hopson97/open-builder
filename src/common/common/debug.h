#pragma once

#include <ostream>
#include <vector>

#include <glm/glm.hpp>

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
