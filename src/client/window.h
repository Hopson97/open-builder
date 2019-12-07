#pragma once


#include "client_engine.h"
#include <SFML/Window/Window.hpp>
#include <common/types.h>

class ClientConfig;
class Keyboard;

class Window {
  public:
    Window(const ClientConfig &config);

    EngineStatus pollEvents(Keyboard &keyboard);

    sf::Window window;
    unsigned width;
    unsigned height;

  private:
    void create(const sf::VideoMode &mode, u32 style);
};