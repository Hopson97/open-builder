#pragma

#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"

/**
 * @brief Render master for the GUIs
 *
 */
class GuiMaster {
  public:
    void addGui();

    void render();

    int getTexture(const std::string& textureName);

  private:
    // Maps a string to a texture index of m_textures
    std::unordered_map<std::string, int> m_textureIds;
    std::vector<gl::Texture2d> m_textures;

    // OpenGL Rendering stuff
    struct {
        gl::Shader program;
        gl::UniformLocation projectionLocation;
        gl::UniformLocation modelLocation;
        gl::UniformLocation colorLocation;
    } m_guiShader;

    // All GUI components use this one quad
    gl::VertexArray m_quadVao;

    glm::mat4 m_projection;
};