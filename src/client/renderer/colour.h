#pragma once

#include <common/types.h>
#include <glm/glm.hpp>

namespace client {
    struct Colour {
        u8 red;
        u8 blue;
        u8 green;

        glm::vec3 asNormalised() const
        {
            return {red / 255.0f, blue / 255.0f, green / 255.0f};
        }
    };

} // namespace client
