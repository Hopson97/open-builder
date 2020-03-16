#pragma once

#include "client_engine.h"
#include "input/keyboard.h"
#include <SFML/Window/Window.hpp>
#include <common/types.h>

struct ClientConfig;

bool createWindowInitOpengl(sf::Window& window, const ClientConfig& config);
float getWindowAspect(const sf::Window& window);
