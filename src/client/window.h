#pragma once

#include "input/keyboard.h"
#include "client_engine.h"
#include <SFML/Window/Window.hpp>
#include <common/types.h>

struct ClientConfig;

/**
 * @brief Provides an interface for creating and common window interactions
 */
class Window {
  public:
    Window(const ClientConfig &config);

    template <typename F, typename F2>
    EngineStatus pollEvents(Keyboard &keyboard, F onKeyRelease, F2 onFocusChange);

    sf::Window window;
    unsigned width;
    unsigned height;
    float aspect;

  private:
    void create(const sf::VideoMode &mode, u32 style);
};

template <typename F, typename F2>
EngineStatus Window::pollEvents(Keyboard &keyboard, F onKeyRelease, F2 onFocusChange)
{
    auto status = EngineStatus::Ok;
    sf::Event e;
    while (window.pollEvent(e)) {
        if (window.hasFocus()) {
            keyboard.update(e);
        }
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Escape) {
                status = EngineStatus::Exit;
            }
        }
        else if (e.type == sf::Event::GainedFocus || e.type == sf::Event::LostFocus) {
            onFocusChange(window);
        }
        else if (e.type == sf::Event::KeyReleased) {
            onKeyRelease(window, e.key.code);
        }
        else if (e.type == sf::Event::Closed) {
            status = EngineStatus::Exit;
        }
    }
    return status;
}