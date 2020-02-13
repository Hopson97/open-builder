#include "gui_rectangle.h"

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
