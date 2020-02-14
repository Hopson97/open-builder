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
    GuiRectangle(const GuiDimension& position, const GuiDimension& size);

    glm::mat4 getRenderTransform(const glm::vec2& viewport) const;

    // Setters rather than "public" as it makes it a lot easier for a clean lua api
    void setPosition(const GuiDimension& position);
    void setSize(const GuiDimension& size);
    void setTexture(int texture);
    void setColour(float r, float g, float b);

    int getTexture() const;
    const glm::vec3& getColour() const;

  private:
    GuiDimension m_position;
    GuiDimension m_size;
    glm::vec3 m_colour{1.0f};

    int m_texture = -1;
};