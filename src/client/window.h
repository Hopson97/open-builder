#pragma once

#include "client_engine.h"
#include "input/keyboard.h"
#include <SFML/Window/Window.hpp>
#include <common/types.h>

struct Window {
    static const sf::Window* context;
    static bool createWindowInitOpengl(sf::Window& window);
    static float getWindowAspect(const sf::Window& window);
};
