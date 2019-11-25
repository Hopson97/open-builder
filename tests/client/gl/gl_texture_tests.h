#include <catch2/catch.hpp>

#include <client/gl/gl_textures.h>

TEST_CASE("Cube textures are correctly created and destroyed")
{
    Texture cube;

    SECTION("The cube texture is created correctly")
    {
        cube = createCubeTexture({"", "", "", "", "", ""});

        REQUIRE(cube.handle != 0);
        REQUIRE(cube.type == TextureType::CubeMap);
    }

    SECTION("The cube texture is destroyed correctly")
    {
        cube = createCubeTexture({"", "", "", "", "", ""});
        destroyTexture(&cube);

		REQUIRE(cube.handle == 0);
	}
}