#include "gui_rectangle.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 GuiRectangle::getRenderTransform(const glm::vec2& viewport) const
{
    glm::vec2 positionTransform = m_position.apply(viewport.x, viewport.y);
    glm::vec2 scaleTransform = m_size.apply(viewport.x, viewport.y);

    glm::mat4 modelMatrix{1.0f};
    modelMatrix =
        glm::translate(modelMatrix, {positionTransform.x, positionTransform.y, 0.0f});
    modelMatrix = glm::scale(modelMatrix, {scaleTransform.x, scaleTransform.y, 0.0f});
    return modelMatrix;
}

void GuiRectangle::setPosition(const GuiDimension& position)
{
    m_position = position;
}

void GuiRectangle::setSize(const GuiDimension& size)
{
    m_size = size;
}

void GuiRectangle::setTexture(int texture)
{
    m_texture = texture;
}

void GuiRectangle::setColour(float r, float g, float b)
{
    m_colour = {r, g, b};
}

int GuiRectangle::getTexture() const
{
    return m_texture;
}

const glm::vec3& GuiRectangle::getColour() const
{
    return m_colour;
}

void GuiRectangle::hide()
{
    m_isHidden = true;
}

void GuiRectangle::show()
{
    m_isHidden = false;
}

bool GuiRectangle::isHidden() const
{
    return m_isHidden;
}