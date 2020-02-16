#include <catch2/catch.hpp>

#include <common/obd_parser.h>

const std::string dataObd = "                               \n\
voxel                           \n\
    name grass                  \n\
    texture_top pack:grass      \n\
    texture_side pack:grasside  \n\
    texture_bottom pack:dirt    \n\
    mesh voxel                  \n\
    state solid                 \n\
end                             \n\
                                \n\
voxel                           \n\
    name dirt                   \n\
                                \n\
    texture_top pack:dirt       \n\
    mesh cross                  \n\
    state flora                 \n\
end                             \n\
";

TEST_CASE("OBD files can be read from correctly")
{
    auto data = getObdDataRaw(dataObd);

    SECTION("There are a correct number of parsed voxels")
    {
        REQUIRE(data.size() == 2);

        REQUIRE(data[0].data.size() == 6);
        REQUIRE(data[1].data.size() == 4);
    }

    SECTION("It correctly has data")
    {
        auto voxel = data[0];

        REQUIRE(voxel.data["name"] == "grass");
        REQUIRE(voxel.data["texture_top"] == "pack:grass");
        REQUIRE(voxel.data["texture_side"] == "pack:grasside");
        REQUIRE(voxel.data["texture_bottom"] == "pack:dirt");
        REQUIRE(voxel.data["mesh"] == "voxel");
        REQUIRE(voxel.data["state"] == "solid");

        auto voxel2 = data[1];

        REQUIRE(voxel2.data["name"] == "dirt");
        REQUIRE(voxel2.data["texture_top"] == "pack:dirt");
        REQUIRE(voxel2.data["mesh"] == "cross");
        REQUIRE(voxel2.data["state"] == "flora");
    }
}