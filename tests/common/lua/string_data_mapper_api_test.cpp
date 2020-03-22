#include <catch2/catch.hpp>

#include <common/lua/script_engine.h>
#include <common/util/string_data_mapper.h>

namespace 
{
    const char* script = R"(
        function test(data)
            local a = data:getInt("a")
            local b = data:getInt("b")
            return a + b
        end
        
        function passer()
            return "a=5;b=10"
        end)";
} // namespace 


TEST_CASE("StringDataMapper Lua Test")
{
    ScriptEngine engine;

    SECTION("Data can be passed into the Lua to get a result")
    {
        StringDataMapper mapper;
        engine.runLuaString(script);

        const char* dataString = "a=5;b=10";
        mapper.parse(dataString);

        int result = engine.lua["test"](mapper);
        REQUIRE(result == 15);
    }


    SECTION("Data can be passed into the Lua to get a result, after getting the dat string from Lua")
    {
        StringDataMapper mapper;
        engine.runLuaString(script);

        std::string data = engine.lua["passer"]();
        mapper.parse(data);

        int result = engine.lua["test"](mapper);
        REQUIRE(result == 15);
    }
}