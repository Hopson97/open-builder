#pragma once

#include "gui_dimension.h"
#include <SFML/Graphics/Rect.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

struct GuiRectangle final {
  public:
    GuiRectangle(const glm::vec2& viewport);
    void updateViewport(const glm::vec2& viewport);

    glm::mat4 getRenderTransform() const;

    // Setters rather than "public" as it makes it a lot easier for a clean lua api
    void setPosition(const GuiDimension& position);
    void setSize(const GuiDimension& size);
    void setTexture(int texture);
    void setColour(float r, float g, float b);

    int getTexture() const;
    const glm::vec3& getColour() const;

    void hide();
    void show();
    bool isHidden() const;

  private:
    void updateBounds();

    sf::FloatRect m_bounds;
    glm::vec2 m_viewport;
    GuiDimension m_position;
    GuiDimension m_size;
    glm::vec3 m_colour{1.0f};

    int m_texture = -1;

    bool m_isHidden = false;
};