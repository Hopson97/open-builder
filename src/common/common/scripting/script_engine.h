#pragma once

#include <sol/sol.hpp>

struct ScriptEngine {
    ScriptEngine();

    void runLuaScript(const std::string &path);

    template <typename... Args>
    auto addTable(const std::string &tableName, Args &&... args)
    {
        return gameTable.create_named(tableName, std::forward<Args>(args)...);
    }

    sol::state lua;
    sol::table gameTable;
};