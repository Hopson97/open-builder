#include "rectangle_component.h"

#include "../gui_constants.h"
#include <glm/gtc/matrix_transform.hpp>

namespace gui {

    glm::mat4 RectangleComponent::getRenderTransform() const
    {
        glm::vec2 positionTransform = m_position.apply();
        glm::vec2 scaleTransform = m_size.apply();

        glm::mat4 modelMatrix{1.0f};
        modelMatrix =
            glm::translate(modelMatrix, {positionTransform.x, positionTransform.y, 0.0f});
        modelMatrix = glm::scale(modelMatrix, {scaleTransform.x, scaleTransform.y, 0.0f});
        return modelMatrix;
    }

    void RectangleComponent::setPosition(const GuiDimension& position)
    {
        m_position = position;
        updateBounds();
    }

    void RectangleComponent::setSize(const GuiDimension& size)
    {
        m_size = size;
        updateBounds();
    }

    void RectangleComponent::setTexture(int texture)
    {
        m_texture = texture;
    }

    int RectangleComponent::getTexture() const
    {
        return m_texture;
    }

    bool RectangleComponent::isInBounds(float x, float y) const
    {
        return m_bounds.contains({x, y});
    }

    void RectangleComponent::updateBounds()
    {
        auto topLeft = m_position.apply();
        auto size = m_size.apply();
        m_bounds = {topLeft.x, GUI_VIEWPORT.y - topLeft.y - size.y, size.x, size.y};
    }

    const sf::FloatRect& RectangleComponent::getBounds() const
    {
        return m_bounds;
    }

} // namespace gui
