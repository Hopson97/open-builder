#include "script_engine.h"

#include <iostream>

ScriptEngine::ScriptEngine()
    : gameTable(lua["game"].get_or_create<sol::table>())
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
}

bool ScriptEngine::runString(const std::string &script)
{
    auto result = lua.script(script, [](auto L, auto res) {
        (void)L;
        return res;
    });

    if (result.valid()) {
        std::cout << "Script string ran sucessfully." << '\n';
    }
    else {
        sol::error err = script;
        std::cerr << "Lua script string invalid. Error: " << err.what() << '\n';
    }
    return result.valid();
}

bool ScriptEngine::runFile(const std::string &path)
{
    auto script = lua.load_file(path);
    if (script.valid()) {
        auto result = script();

        if (result.valid()) {
            std::cout << "Script ran sucessfully: " << path << '\n';
            return true;
        }
        else {
            sol::error err = result;
            std::cerr << "Lua error: " << err.what() << std::endl;
        }
    }
    else {
        sol::error err = script;
        std::cerr << "Lua script file invalid: " << path
                  << "Error: " << err.what() << '\n';
    }
    return false;
}

sol::function ScriptEngine::getLuaFunction(const char *functionName)
{
    return gameTable[functionName];
}
