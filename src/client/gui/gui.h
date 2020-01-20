#pragma once

#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
#include <SFML/Window/Event.hpp>
#include <common/macros.h>
#include <memory>
#include <vector>

struct GuiImage {
    gl::Texture2d m_image;

    void setSource(const std::string &imageSource);
};

class Gui final {
  public:
    NON_COPYABLE(Gui)
    NON_MOVEABLE(Gui)

    Gui();
    ~Gui();
    void processKeypress(sf::Event e);
    void processMouseEvent(sf::Event e);

    void addImage(const GuiImage &image);

    void render();

  private:
    struct {
        gl::Shader program;
        gl::UniformLocation positionLocation;
        gl::UniformLocation scaleLocation;
    } m_guiShader;

    gl::VertexArray m_quad;
    std::vector<GuiImage> m_images;
};