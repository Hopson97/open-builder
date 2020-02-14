#pragma once

#include <glm/vec2.hpp>
#include <vector>

class GuiRectangle;
class GuiShader;

namespace gl {
class Drawable;
}

class GuiContainer final {
  public:
    GuiContainer();

    void hide();
    void show();

    void addRectangle(GuiRectangle& rectangle);

    void render(GuiShader& shader, const glm::vec2& viewport, const gl::Drawable& quad);

  private:
    static int uidCount;
    int m_uid;

    // Stored by lua
    std::vector<GuiRectangle*> m_guiRectangles;

    bool m_isHidden = true;
};
