#pragma once

#include "../gl/vertex_array.h"
#include "gui_component.h"
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>

class Gui final {
  public:
    Gui();
    ~Gui();
    void processKeypress(sf::Event e);
    void processMouseEvent(sf::Event e);

    void render();

  private:
    gl::VertexArray m_quad;
    std::vector<std::unique_ptr<GuiComponent>> m_components;
};