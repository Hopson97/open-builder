#include "window.h"

#include "client_config.h"
#include <SFML/Window/Event.hpp>
#include "input/keyboard.h"


Window::Window(const ClientConfig &config)
{
    window.setKeyRepeatEnabled(false);
    if (config.fullScreen) {
        create(sf::VideoMode::getDesktopMode(), sf::Style::Fullscreen);
        width = window.getSize().x;
        height = window.getSize().y;
    }
    else {
        width = static_cast<unsigned>(config.windowWidth);
        height = static_cast<unsigned>(config.windowHeight);
        create({width, height}, sf::Style::Close);
    }
    if (config.isFpsCapped) {
        window.setFramerateLimit(config.fpsLimit);
    }
}

EngineStatus Window::pollEvents(Keyboard &keyboard)
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
        else if (e.type == sf::Event::KeyReleased) {
            //  onKeyRelease(e.key.code);
        }
        else if (e.type == sf::Event::Closed) {
            status = EngineStatus::Exit;
        }
    }
    return status;
}


void Window::create(const sf::VideoMode &mode, u32 style)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    window.create(mode, "Open Builder", style, settings);
}
