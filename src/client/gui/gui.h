#pragma once

#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
#include <SFML/Window/Event.hpp>
#include <common/macros.h>
#include <common/scripting/script_engine.h>
#include <memory>
#include <vector>

class Text;

// Todo: Maybe move these out into their own files

// Stores the scale and offset of a GUI element in 2 dimensions
struct GuiDimension {
    glm::vec2 scale{0.0f};
    glm::vec2 offset{0.0f};

    GuiDimension() = default;
    GuiDimension(float xScale, float xOffset, float yScale, float yOffset);

    glm::vec2 apply(float width, float height);
};

// GUI Image,
struct GuiImage {
    gl::Texture2d texture;

    GuiDimension position;
    GuiDimension size;

    glm::vec3 colour{0.0f};

    void setSource(const std::string& imageSource);

    void setColour(float r, float g, float b);

    void setPixelSize(float width, float height);
    void setScaledSize(float width, float height);

    void setPixelOffset(float x, float y);
    void setScaledPosition(float x, float y);
};

class Gui final {
  public:
    Gui(float windowWidth, float windowHeight);

    void processKeypress(sf::Event e);
    void processMouseEvent(sf::Event e);

    void addImage(sol::userdata image);

    void render(float width, float height);

    void renderText(Text& text);

  private:
    struct {
        gl::Shader program;
        gl::UniformLocation projectionLocation;
        gl::UniformLocation modelLocation;
        gl::UniformLocation colorLocation;
    } m_guiShader;

    glm::mat4 m_orthoMatrix;
    gl::VertexArray m_quad;
    std::vector<sol::userdata> m_images;
};

sol::table createGuiApi(ScriptEngine& engine);