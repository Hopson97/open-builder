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

    /**
     * @brief Creates a rectangle object and returns it.
     *  The lifetime of the object is handled by this class
     * @return GuiRectangle*
     */
    GuiRectangle* addRectangle();

    void render(GuiShader& shader, const glm::vec2& viewport, const gl::Drawable& quad,
                const std::vector<gl::Texture2d>& textures);

  private:
    static int uidCount;
    int m_uid;

    // Stored as unique ptr to ensure lifetime of the pointer returned
    // so that Lua is able reference elements as the vector grows
    std::vector<std::unique_ptr<GuiRectangle>> m_guiRectangles;

    bool m_isHidden = true;
};
