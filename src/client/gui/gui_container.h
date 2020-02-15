#pragma once

#include "../gl/textures.h"
#include "gui_rectangle.h"
#include <glm/vec2.hpp>
#include <memory>
#include <vector>

class GuiShader;

namespace gl {
class Drawable;
}

class GuiContainer final {
  public:
    GuiContainer();

    void hide();
    void show();

    GuiRectangle* addRectangle();

    void render(GuiShader& shader, const glm::vec2& viewport, const gl::Drawable& quad,
                const std::vector<gl::Texture2d>& textures);

  private:
    static int uidCount;
    int m_uid;

    std::vector<GuiRectangle> m_guiRectangles;

    bool m_isHidden = true;
};
