#pragma once

#include "client_engine.h"
#include "input/keyboard.h"
#include <SFML/Window/Window.hpp>
#include <common/types.h>

bool createWindowInitOpengl(sf::Window& window);
float getWindowAspect(const sf::Window& window);
