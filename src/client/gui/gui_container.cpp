#include "gui_container.h"

#include "../gl/vertex_array.h"
#include "gui_rectangle.h"
#include "gui_shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int GuiContainer::uidCount = 0;

GuiContainer::GuiContainer()
    : m_uid(uidCount++)
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

void GuiContainer::render(GuiShader& shader, const glm::vec2& viewport,
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