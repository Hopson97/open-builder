#include "gui_container.h"

#include "../gl/vertex_array.h"
#include "gui_rectangle.h"
#include "gui_shader.h"
#include "gui_text.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int GuiContainer::uidCount = 0;

GuiContainer::GuiContainer(gl::Font& font)
    : m_uid(uidCount++)
    , mp_font(&font)
{
}

void GuiContainer::hide()
{
    m_isHidden = true;
}

void GuiContainer::show()
{
    m_isHidden = false;
}

GuiRectangle* GuiContainer::addRectangle()
{
    return m_guiRectangles.emplace_back(std::make_unique<GuiRectangle>()).get();
}

GuiText* GuiContainer::addText()
{
    return m_guiTexts.emplace_back(std::make_unique<GuiText>(*mp_font)).get();
}

void GuiContainer::renderRects(GuiShader& shader, const glm::vec2& viewport,
                               const gl::Drawable& quad,
                               const std::vector<gl::Texture2d>& textures)
{
    // TODO Maybe render to a framebuffer to avoid having to bind a bunch of textures over
    // and over

    for (auto& rect : m_guiRectangles) {
        auto transform = rect->getRenderTransform(viewport);
        shader.updateTransform(transform);

        auto& colour = rect->getColour();
        shader.updateColour(colour);

        int texture = rect->getTexture();
        if (texture > -1) {
            textures.at(texture).bind();
        }
        quad.draw();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void GuiContainer::renderText(GuiShader& shader, const glm::vec2& viewport)
{
    for (auto& text : m_guiTexts) {
        text->render(shader, viewport);
    }
}