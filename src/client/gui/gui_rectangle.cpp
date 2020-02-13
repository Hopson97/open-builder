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

GuiRectangle::GuiRectangle(const GuiDimension& position, const GuiDimension& scale)
    : m_position(position)
    , m_scale(scale)
{
}

glm::mat4 GuiRectangle::getRenderTransform(const glm::vec2& viewport) const
{
    glm::vec2 position = m_position.apply(viewport.x, viewport.y);
    glm::vec2 scale = m_scale.apply(viewport.x, viewport.y);

    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, {position.x, position.y, 0.0f});
    modelMatrix = glm::scale(modelMatrix, {scale.x, scale.y, 0.0f});

    return modelMatrix;
}