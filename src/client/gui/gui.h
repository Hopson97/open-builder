#pragma once

#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
#include <SFML/Window/Event.hpp>
#include <common/scripting/script_engine.h>
#include <common/macros.h>
#include <memory>
#include <vector>

struct GDim {
    sf::Vector2f scale;
    sf::Vector2f offset;

    GDim() 
        : scale(sf::Vector2f(0, 0)), offset(sf::Vector2f(0, 0)) {}
    GDim(float x_s, float x_o, float y_s, float y_o)
        : scale(sf::Vector2f(x_s, y_s)), offset(sf::Vector2f(x_o, y_o)) {}
};

struct GuiImage {
    gl::Texture2d m_image;
    GDim m_size;
    GDim m_position;

    void setSource(const std::string &imageSource);
    void setSize(GDim new_size);
    void setPosition(GDim new_pos);
};

class Gui final {
  public:
    NON_COPYABLE(Gui)
    NON_MOVEABLE(Gui)

    Gui();
    ~Gui();
    void addUsertypes(sol::table& m_lua);

    void processKeypress(sf::Event e);
    void processMouseEvent(sf::Event e);

    void addImage(GuiImage &image);

    void render(int width, int height);


  private:
    struct {
        gl::Shader program;
        gl::UniformLocation modelLocation;
        gl::UniformLocation positionLocation;
        gl::UniformLocation scaleLocation;
    } m_guiShader;

    gl::VertexArray m_quad;
    std::vector<GuiImage*> m_images;
};