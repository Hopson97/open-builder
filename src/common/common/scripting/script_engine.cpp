#include "script_engine.h"

#include <iostream>

ScriptEngine::ScriptEngine()
    : gameTable(lua["openbuilder"].get_or_create<sol::table>())
{
    lua.open_libraries(sol::lib::base);
}

void ScriptEngine::runLuaScript(const std::string &path)
{
    auto script = lua.load_file(path);
    if (script.valid()) {
        auto result = script();
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "Lua error: " << err.what() << std::endl;
        }
    }
    else {
        sol::error err = script;
        std::cerr << "Lua script file invalid: " << path
                  << "Error: " << err.what() << '\n';
    }
}
