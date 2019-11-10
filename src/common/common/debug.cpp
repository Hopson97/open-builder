#include "debug.h"

std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vec)
{
    stream << "X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z;
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const glm::vec2 &vec)
{
    stream << "X: " << vec.x << " Y: " << vec.y;
    return stream;
}
