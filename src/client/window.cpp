#include "window.h"

#include "client_config.h"
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>

namespace {

    void createWindow(sf::Window& window, const sf::VideoMode& mode, u32 style)
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
        window.setFramerateLimit(60);
    }

} // namespace

void createWindow(sf::Window& window, const ClientConfig& config)
{
    window.setKeyRepeatEnabled(false);
    if (config.fullScreen) {
        createWindow(window, sf::VideoMode::getDesktopMode(), sf::Style::Fullscreen);
    }
    else {
        unsigned width = static_cast<unsigned>(config.windowWidth);
        unsigned height = static_cast<unsigned>(config.windowHeight);
        createWindow(window, {width, height}, sf::Style::Close);
    }
    if (config.isFpsCapped) {
        window.setFramerateLimit(config.fpsLimit);
    }
}

float getWindowAspect(const sf::Window& window)
{
    return static_cast<float>(window.getSize().x) /
           static_cast<float>(window.getSize().y);
}
