#pragma once

#include <catch2/catch.hpp>
#include <client/gl/gl_shader.h>

TEST_CASE("Shaders are correctly created and destroyed")
{
    Shader shader = createShader("static", "static");

    REQUIRE(shader.program != 0);

    destroyShader(&shader);

    REQUIRE(shader.program == 0);
}

TEST_CASE("Uniform locations are successfully found")
{
    Shader shader = createShader("static", "static");
    UniformLocation location = getUniformLocation(shader, "modelMatrix");

	 REQUIRE(location.ptr != 0);
}