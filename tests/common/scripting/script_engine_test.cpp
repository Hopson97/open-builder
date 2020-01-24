#include <catch2/catch.hpp>
#include <common/scripting/script_engine.h>

const char* SCRIPT = R"(
function game.square(a)
    return a * a
end
)";

TEST_CASE("Script engine tests")
{
    ScriptEngine script;

    SECTION("By default, scripts have a 'game' global table")
    {
        REQUIRE(script.lua["game"].valid());
    }   

    SECTION("Invalid scipts do not run")
    {
        REQUIRE(!script.runString("fhfjchwu"));
    }

    SECTION("Valid scripts are ran")
    {
        REQUIRE(script.runString(SCRIPT));
    }

    SECTION("Functions can be found")
    {
        script.runString(SCRIPT);
        auto function = script.getLuaFunction("square");
        REQUIRE(function.valid());
    }

    SECTION("Functions can be ran")
    {
        script.runString(SCRIPT);
        auto function = script.getLuaFunction("square");
        int result = function(5);
        REQUIRE(result == 25);
    }

    SECTION("Functions can be found safely")
    {
        script.runString(SCRIPT);
        auto result = script.runLuaFunctionSafe<int>("square", 5);
        REQUIRE(*result == 25);
    }

    SECTION("Functions that do not exist can be ID'd")
    {
        script.runString(SCRIPT);
        auto result = script.runLuaFunctionSafe<int>("cube", 5);
        REQUIRE(!result);
    }
}