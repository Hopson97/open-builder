#pragma once

#include <glm/vec2.hpp>

struct GuiDimension final {
    glm::vec2 scale{0.0f};
    glm::vec2 offset{0.0f};

    GuiDimension() = default;

    GuiDimension(float xScale, float xOffset, float yScale, float yOffset)
        : scale{xScale, yScale}
        , offset{xOffset, yOffset}
    {
    }

    glm::vec2 apply(float width, float height) const
    {
        glm::vec2 vector;
        vector.x = scale.x * width * 100.0f;
        vector.y = scale.y * height * 100.0f;
        return vector + offset;
    }
};