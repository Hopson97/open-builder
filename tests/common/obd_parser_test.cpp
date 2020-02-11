#include <catch2/catch.hpp>

#include <common/obd_parser.h>
#include <iostream>

const std::string dataObd = 
"\n\
cap_fps 0 \n\
\n\
fullscreen 0 \n\
\n\
   \n\
\n\
key1 value1 \n\
";  

TEST_CASE("OBD files can be read from correctly")
{
    auto data = parseObdData(dataObd);

    SECTION("There are a correct number of parsed blocks")
    {
        REQUIRE(data.size() == 3);
    }

    SECTION("It correctly has data")
    {
		REQUIRE(data["cap_fps"] == "0");
		REQUIRE(data["fullscreen"] == "0");
		REQUIRE(data["key1"] == "value1");
    }
}
