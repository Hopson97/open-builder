#include <catch2/catch.hpp>
#include <common/lua/script_engine.h>

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

    SECTION("Invalid scripts return false")
    {
        REQUIRE(!script.runLuaString("fhfjchwu"));
    }

    SECTION("Valid scripts return true")
    {
        REQUIRE(script.runLuaString(SCRIPT));
    }

    SECTION("Lua functions can be found and ran")
    {
        script.runLuaString(SCRIPT);
        auto square = script.getLuaFunction("square");
        int result = square(5);
        REQUIRE(result == 25);
    }

    SECTION("Functions can be found safely")
    {
        script.runLuaString(SCRIPT);
        auto result = script.runLuaFunctionSafe<int>("square", 5);
        REQUIRE(*result == 25);
    }

    SECTION("Functions that do not exist can be ID'd")
    {
        script.runLuaString(SCRIPT);
        auto result = script.runLuaFunctionSafe<int>("cube", 5);
        REQUIRE(!result.has_value());
    }
}
