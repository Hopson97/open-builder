#include "gui_container.h"

#include "../gl/vertex_array.h"
#include "gui_rectangle.h"
#include "gui_shader.h"
#include "gui_text.h"
#include <glm/gtc/matrix_transform.hpp>

int GuiContainer::uidCount = 0;

GuiContainer::GuiContainer(gl::Font& font, const glm::vec2& viewport)
    : m_uid(uidCount++)
    , mp_font(&font)
    , m_viewport(viewport)
{
}

GuiRectangle* GuiContainer::addRectangle()
{
    return m_guiRectangles.emplace_back(std::make_unique<GuiRectangle>(m_viewport)).get();
}

GuiText* GuiContainer::addText()
{
    return m_guiTexts.emplace_back(std::make_unique<GuiText>(*mp_font)).get();
}

void GuiContainer::renderRects(GuiShader& shader, const gl::Drawable& quad,
                               const std::vector<gl::Texture2d>& textures)
{
    // TODO Maybe render to a framebuffer to avoid having to bind a bunch of textures over
    // and over

    for (auto& rect : m_guiRectangles) {
        if (rect->isHidden()) {
            continue;
        }
        auto transform = rect->getRenderTransform();
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

void GuiContainer::renderText(GuiShader& shader)
{
    auto viewport = m_viewport / 100.0f;
    for (auto& text : m_guiTexts) {
        text->render(shader, viewport);
    }
}

void GuiContainer::show()
{
    m_isHidden = false;
}

void GuiContainer::hide()
{
    m_isHidden = true;
}

bool GuiContainer::isHidden() const
{
    return m_isHidden;
}
