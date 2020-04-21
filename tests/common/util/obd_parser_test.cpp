#include <catch2/catch.hpp>

#include <common/util.h>

const std::string dataObd = "   \n\
    name grass                  \n\
    texture_top pack:grass      \n\
    texture_side pack:grasside  \n\
    texture_bottom pack:dirt    \n\
    mesh voxel                  \n\
    state solid                 \n\
";

TEST_CASE("OBD files can be read from correctly")
{
    auto data = getObdDataRaw(dataObd);

    SECTION("There are a correct number of parsed data")
    {
        REQUIRE(data.size() == 6);
    }

    SECTION("It correctly has data")
    {
        REQUIRE(data["name"] == "grass");
        REQUIRE(data["texture_top"] == "pack:grass");
        REQUIRE(data["texture_side"] == "pack:grasside");
        REQUIRE(data["texture_bottom"] == "pack:dirt");
        REQUIRE(data["mesh"] == "voxel");
        REQUIRE(data["state"] == "solid");
    }
}
