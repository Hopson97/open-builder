#include "rectangle_component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace gui {

glm::mat4 RectangleComponent::getRenderTransform(const glm::vec2& viewport) const
{
    auto scaledViewport = viewport / 100.0f;
    glm::vec2 positionTransform = m_position.apply(scaledViewport);
    glm::vec2 scaleTransform = m_size.apply(scaledViewport);

    glm::mat4 modelMatrix{1.0f};
    modelMatrix =
        glm::translate(modelMatrix, {positionTransform.x, positionTransform.y, 0.0f});
    modelMatrix = glm::scale(modelMatrix, {scaleTransform.x, scaleTransform.y, 0.0f});
    return modelMatrix;
}

void RectangleComponent::setPosition(const GuiDimension& position)
{
    m_position = position;
}

void RectangleComponent::setSize(const GuiDimension& size)
{
    m_size = size;
}

void RectangleComponent::setTexture(int texture)
{
    m_texture = texture;
}

void RectangleComponent::setColour(float r, float g, float b)
{
    m_colour = {r, g, b};
}

int RectangleComponent::getTexture() const
{
    return m_texture;
}

const glm::vec3& RectangleComponent::getColour() const
{
    return m_colour;
}

bool RectangleComponent::isInBounds(float x, float y) const
{
    return m_bounds.contains({x, y});
}

void RectangleComponent::updateBounds(const glm::vec2& viewport)
{
    auto scaledViewport = viewport / 100.0f;
    auto topLeft = m_position.apply(scaledViewport);
    auto size = m_size.apply(scaledViewport);
    m_bounds = {topLeft.x, viewport.y - topLeft.y - size.y, size.x, size.y};
}

} // namespace gui