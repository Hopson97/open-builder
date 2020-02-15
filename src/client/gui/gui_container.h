#pragma once

#include "../gl/textures.h"
#include "gui_rectangle.h"
#include "gui_text.h"
#include <glm/vec2.hpp>
#include <memory>
#include <vector>

class Font;
class GuiShader;

namespace gl {
class Drawable;
}

class GuiContainer final {
  public:
    GuiContainer(Font& font);

    void hide();
    void show();

    /// lifetime managed by the gui container (this)
    GuiRectangle* addRectangle();
    GuiText* addText();

    void render(GuiShader& shader, const glm::vec2& viewport, const gl::Drawable& quad,
                const std::vector<gl::Texture2d>& textures);

  private:
    static int uidCount;
    int m_uid;

    // Stored as unique ptr to ensure lifetime of the pointer returned
    // so that Lua is able reference elements as the vector grows
    std::vector<std::unique_ptr<GuiRectangle>> m_guiRectangles;
    std::vector<std::unique_ptr<GuiText>> m_guiTexts;
    Font* mp_font;

    bool m_isHidden = true;
};
