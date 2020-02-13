#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

struct GuiDimension final {
    glm::vec2 scale{0.0f};
    glm::vec2 offset{0.0f};

    GuiDimension() = default;
    GuiDimension(float xScale, float xOffset, float yScale, float yOffset);

    glm::vec2 apply(float width, float height) const;
};

struct GuiRectangle final {
  public:
    GuiRectangle() = default;
    GuiRectangle(const GuiDimension& position, const GuiDimension& scale);

    glm::mat4 getRenderTransform(const glm::vec2& viewport) const;

    GuiDimension position;
    GuiDimension scale;

    int m_texture = 0;
};