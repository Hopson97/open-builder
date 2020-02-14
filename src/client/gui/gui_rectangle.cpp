#include "gui_rectangle.h"

#include <glm/gtc/matrix_transform.hpp>

GuiDimension::GuiDimension(float xScale, float xOffset, float yScale, float yOffset)
    : scale{xScale, yScale}
    , offset{xOffset, yOffset}
{
}

glm::vec2 GuiDimension::apply(float width, float height) const
{
    glm::vec2 vector;
    vector.x = scale.x * width * 100.0f;
    vector.y = scale.y * height * 100.0f;
    return vector + offset;
}

GuiRectangle::GuiRectangle(const GuiDimension& position, const GuiDimension& size)
    : m_position(position)
    , m_size(size)
{
}

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

int GuiRectangle::getTexture() const
{
    return m_texture;
}