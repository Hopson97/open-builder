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

GuiRectangle::GuiRectangle(const GuiDimension& rectPostion, const GuiDimension& rectScale)
    : position(rectPostion)
    , scale(rectScale)
{
}

glm::mat4 GuiRectangle::getRenderTransform(const glm::vec2& viewport) const
{
    glm::vec2 positionTransform = position.apply(viewport.x, viewport.y);
    glm::vec2 scaleTransform = scale.apply(viewport.x, viewport.y);

    glm::mat4 modelMatrix{1.0f};
    modelMatrix =
        glm::translate(modelMatrix, {positionTransform.x, positionTransform.y, 0.0f});
    modelMatrix = glm::scale(modelMatrix, {scaleTransform.x, scaleTransform.y, 0.0f});

    return modelMatrix;
}