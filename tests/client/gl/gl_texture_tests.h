#pragma once

#include <catch2/catch.hpp>

#include <client/gl/gl_textures.h>

TEST_CASE("Cube textures are correctly created")
{
    Texture cube;

    cube = createCubeTexture({"", "", "", "", "", ""});

    REQUIRE(cube.handle != 0);
    REQUIRE(cube.type == TextureType::CubeMap);
}

TEST_CASE("2D textures are correctly created")
{
    Texture texture;
    texture = createTexture2D("");

    REQUIRE(texture.handle != 0);
    REQUIRE(texture.type == TextureType::Texture2d);
}

TEST_CASE("Textures are destroyed correctly")
{
    Texture texture;
    texture = createTexture2D("");
    destroyTexture(&texture);

    REQUIRE(texture.handle == 0);
    REQUIRE(texture.type == TextureType::None);
}