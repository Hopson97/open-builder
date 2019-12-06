#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <glad/glad.h>
#include <SFML/Window/Window.hpp>

#include "server/network/server_test.h"

int main(int argc, char *argv[])
{
    sf::Window window({200, 200}, "Tests");
    gladLoadGL();
    int result = Catch::Session().run(argc, argv);
	window.close();
    return result;
}