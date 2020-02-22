#include "component.h"

#include <glm/gtc/matrix_transform.hpp>

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
    //updateBounds();
}

void RectangleComponent::setSize(const GuiDimension& size)
{
    m_size = size;
    //updateBounds();
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

void RectangleComponent::hide()
{
    m_isHidden = true;
}

void RectangleComponent::show()
{
    m_isHidden = false;
}

bool RectangleComponent::isHidden() const
{
    return m_isHidden;
}
/*
void RectangleComponent::updateBounds()
{
    auto viewport = m_viewport / 100.0f;
    auto topLeft = m_position.apply(viewport);
    auto size = m_size.apply(viewport);

    m_bounds = {topLeft.x, topLeft.y, size.x, size.y};
}*/

} // namespace gui