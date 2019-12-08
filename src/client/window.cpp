#include "window.h"

#include "client_config.h"
#include <SFML/Window/Event.hpp>

Window::Window(const ClientConfig &config)
{
    window.setKeyRepeatEnabled(false);
    if (config.fullScreen) {
        create(sf::VideoMode::getDesktopMode(), sf::Style::Fullscreen);
        width = window.getSize().x;
        height = window.getSize().y;
        aspect = width / height;
    }
    else {
        width = static_cast<unsigned>(config.windowWidth);
        height = static_cast<unsigned>(config.windowHeight);
        aspect = width / height;
        create({width, height}, sf::Style::Close);
    }
    if (config.isFpsCapped) {
        window.setFramerateLimit(config.fpsLimit);
    }
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
