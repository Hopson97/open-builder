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
    GuiRectangle();
    GuiRectangle(const GuiDimension& position, const GuiRectangle& scale);

    glm::mat4 getRenderTransform() const;

  private:
    GuiDimension position;
    GuiDimension scale;
    int texture = 0;
};