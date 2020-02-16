#pragma once

#include "../gl/font.h"
#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
#include "gui_container.h"
#include "gui_shader.h"
#include <memory>
#include <unordered_map>

/**
 * @brief Render master for the GUIs
 *
 */
class GuiMaster final {
  public:
    GuiMaster(float viewportWidth, float viewportHeight);

    GuiContainer* addGui();

    void render();

    int getTexture(const std::string& textureName);

    // For testing purposes
    int guiCount() const;
    int textureCount() const;

  private:
    // Stored as unique ptr to ensure lifetime of the pointer returned
    // so that Lua is able reference elements as the vector grows
    std::vector<std::unique_ptr<GuiContainer>> m_containers;

    // Maps a string to a texture index of m_textures
    std::unordered_map<std::string, int> m_textureIds;
    std::vector<gl::Texture2d> m_textures;
    gl::Font m_font;

    // OpenGL Rendering stuff
    GuiShader m_shader;
    gl::VertexArray m_quadVao;
    glm::mat4 m_projection;
    glm::vec2 m_viewport;
};