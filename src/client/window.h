#pragma once

#include "client_engine.h"
#include "input/keyboard.h"
#include <SFML/Window/Window.hpp>
#include <common/types.h>

struct ClientConfig;

/**
 * @brief Provides an interface for creating and common window interactions
 */
class Window {
  public:
    Window(const ClientConfig &config);

    template <typename F, typename MouseRelease>
    EngineStatus pollEvents(Keyboard &keyboard, F onKeyRelease,
                            MouseRelease onMouseRelease);

    sf::Window window;
    unsigned width;
    unsigned height;
    float aspect;

  private:
    void create(const sf::VideoMode &mode, u32 style);
};

template <typename F, typename MouseRelease>
EngineStatus Window::pollEvents(Keyboard &keyboard, F onKeyRelease,
                                MouseRelease onMouseRelease)
{
    auto status = EngineStatus::Ok;
    sf::Event e;
    while (window.pollEvent(e)) {
        if (window.hasFocus()) {
            keyboard.update(e);
        }

        switch (e.type) {
            case sf::Event::KeyPressed:
                if (e.key.code == sf::Keyboard::Escape) {
                    status = EngineStatus::Exit;
                }
                break;

            case sf::Event::KeyReleased:
                onKeyRelease(e.key.code);
                break;

            case sf::Event::MouseButtonReleased:
                onMouseRelease(e.mouseButton.x, e.mouseButton.y);
                break;

            case sf::Event::Closed:
                status = EngineStatus::Exit;
                break;

            default:
                break;
        }
    }
    return status;
}