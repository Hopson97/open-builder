#include <common/util/string_data_mapper.h>

#include <catch2/catch.hpp>

TEST_CASE("String data mapper test")
{
    StringDataMapper mapper;

    const char* string = "hello=world;size=30";

    SECTION("Values can be read from the string by the parser")
    {
        mapper.parse(string);
        REQUIRE(mapper.getString("hello") == "world");
        REQUIRE(mapper.getInt("size") == 30);
    }
}