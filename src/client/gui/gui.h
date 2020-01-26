#pragma once

#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
#include <SFML/Window/Event.hpp>
#include <common/scripting/script_engine.h>
#include <common/macros.h>
#include <memory>
#include <vector>

// Todo: Maybe move these out into their own files 

// Stores the scale and offset of a GUI element in 2 dimensions
struct GDim {
    sf::Vector2f scale;
    sf::Vector2f offset;

    GDim() 
        : scale(sf::Vector2f(0, 0)), offset(sf::Vector2f(0, 0)) {}

    GDim(float x_s, float x_o, float y_s, float y_o)
        : scale(sf::Vector2f(x_s, y_s)), offset(sf::Vector2f(x_o, y_o)) {}
};

// GUI Color, each colour ranges from 0-1 (floats)
struct Color3 {
    float r, g, b;

    Color3()                          // Default should be 1,1,1 so scripters don't need
        : r(1.f), g(1.f), b(1.f) {}   // to set color just to see the image
                                      
    Color3(float r_, float g_, float b_)
        : r{r_}, g{g_}, b{b_} {}
};

// GUI Image, 
struct GuiImage {
    gl::Texture2d m_image;
    GDim m_size;
    GDim m_position;
    Color3 m_color;

    // Setters (TODO: Getters?)
    void setSource(const std::string &imageSource);
    void setSize(GDim new_size);
    void setPosition(GDim new_pos);
    void setColor(Color3 new_color);
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

    void addImage(sol::userdata image);

    void render(int width, int height);


  private:
    struct {
        gl::Shader program;
        gl::UniformLocation modelLocation;
        gl::UniformLocation colorLocation;
    } m_guiShader;

    gl::VertexArray m_quad;
    std::vector<sol::userdata> m_images;
};