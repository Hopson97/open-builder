#include "window.h"

#include "client_config.h"
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>

Window::Window(const ClientConfig& config)
{
    window.setKeyRepeatEnabled(false);
    if (config.get_fullScreen()) {
        create(sf::VideoMode::getDesktopMode(), sf::Style::Fullscreen);
        width = window.getSize().x;
        height = window.getSize().y;
        aspect = static_cast<float>(width) / static_cast<float>(height);
    }
    else {
        width = static_cast<unsigned>(config.get_windowWidth());
        height = static_cast<unsigned>(config.get_windowHeight());
        aspect = static_cast<float>(width) / static_cast<float>(height);
        create({width, height}, sf::Style::Close);
    }
    if (config.get_isFpsCapped()) {
        window.setFramerateLimit(config.get_fpsLimit());
    }
}

void Window::create(const sf::VideoMode& mode, u32 style)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;
#ifndef NDEBUG
    settings.attributeFlags |= sf::ContextSettings::Debug;
#else
    if (GLAD_GL_KHR_no_error)
        settings.attributeFlags |= GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR;
#endif

    window.create(mode, "Open Builder", style, settings);
}
