#include "window.h"

#include <iostream>

#include <glad/glad.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "client_config.h"
#include "gl/gl_errors.h"

const sf::Window* Window::context = nullptr;

namespace {

    bool initOpenGL(const sf::Window& window)
    {
        if (!gladLoadGL()) {
            return false;
        }
        initGLDebug();
        glCheck(glClearColor(0.25f, 0.75f, 1.0f, 0.0f));
        glCheck(glViewport(0, 0, window.getSize().x, window.getSize().y));
        glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        glCheck(glEnable(GL_DEPTH_TEST));
        return true;
    }

    void createWindow(sf::Window& window, const sf::VideoMode& mode, u32 style, sf::State state)
    {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antiAliasingLevel = 4;
        settings.majorVersion = 4;
        settings.minorVersion = 6;
#ifndef NDEBUG
        settings.attributeFlags |= sf::ContextSettings::Debug;
#else
        if (GLAD_GL_KHR_no_error)
            settings.attributeFlags |= GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR;
#endif

        window.create(mode, "Open Builder", style, state, settings);
        window.setPosition({window.getPosition().x, 0});
        window.setVerticalSyncEnabled(true);
        //window.setFramerateLimit(60);
    }

} // namespace

bool Window::createWindowInitOpengl(sf::Window& window)
{
    context = &window;
    window.setKeyRepeatEnabled(false);
    if (ClientConfig::get().fullScreen) {
        createWindow(window, sf::VideoMode::getDesktopMode(), sf::Style::None, sf::State::Fullscreen);
    }
    else {
        unsigned width = static_cast<unsigned>(ClientConfig::get().windowWidth);
        unsigned height = static_cast<unsigned>(ClientConfig::get().windowHeight);
        createWindow(window, sf::VideoMode{{width, height}}, sf::Style::Close, sf::State::Windowed);
    }
    if (ClientConfig::get().isFpsCapped) {
        window.setFramerateLimit(ClientConfig::get().fpsLimit);
    }

    return initOpenGL(window);
}

float Window::getWindowAspect(const sf::Window& window)
{
    return static_cast<float>(window.getSize().x) /
           static_cast<float>(window.getSize().y);
}
