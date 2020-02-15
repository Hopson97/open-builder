#pragma once

#include "../gl/textures.h"
#include "gui_rectangle.h"
#include "gui_text.h"
#include <glm/vec2.hpp>
#include <memory>
#include <vector>

class GuiShader;

namespace gl {
class Drawable;
class Font;
} // namespace gl

class GuiContainer final {
  public:
    GuiContainer(gl::Font& font);

    void hide();
    void show();

    /// lifetime managed by the gui container (this)
    GuiRectangle* addRectangle();
    GuiText* addText();

    void renderRects(GuiShader& shader, const glm::vec2& viewport,
                     const gl::Drawable& quad,
                     const std::vector<gl::Texture2d>& textures);

    void renderText(GuiShader& shader, const glm::vec2& viewport);

  private:
    static int uidCount;
    int m_uid;

    // Stored as unique ptr to ensure lifetime of the pointer returned
    // so that Lua is able reference elements as the vector grows
    std::vector<std::unique_ptr<GuiRectangle>> m_guiRectangles;
    std::vector<std::unique_ptr<GuiText>> m_guiTexts;
    gl::Font* mp_font;

    bool m_isHidden = true;
};
