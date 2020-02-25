#pragma once

#include "component.h"
#include <SFML/Graphics/Rect.hpp>

namespace gui {

class RectangleComponent final : public Component {
  public:
    glm::mat4 getRenderTransform() const;

    void setPosition(const GuiDimension& position) final override;
    void setSize(const GuiDimension& size) final override;
    void setTexture(int texture);
    void setColour(float r, float g, float b);

    int getTexture() const;
    const glm::vec3& getColour() const;

    bool isInBounds(float x, float y) const;

    const sf::FloatRect& getBounds() const;

  private:
    void updateBounds();

    sf::FloatRect m_bounds;
    GuiDimension m_position;
    GuiDimension m_size;
    glm::vec3 m_colour{1.0f};

    int m_texture = -1;
};
} // namespace gui
