#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <SFML/Window/Window.hpp>
#include <glad/glad.h>

#include "common/network/net_host_test.h"
#include "common/world/chunk_manager_test.h"
#include "common/world/chunk_test.h"
#include "common/world/coordinate_test.h"

// ENet causes a bunch compiler warnings eg unused vars so these are here to
// hide that
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#define ENET_IMPLEMENTATION
#include <common/network/enet.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

int main(int argc, char *argv[])
{
    // sf::Window window({200, 200}, "Tests");
    // gladLoadGL();
    enet_initialize();
    int result = Catch::Session().run(argc, argv);
    // window.close();
    enet_deinitialize();
    return result;
}