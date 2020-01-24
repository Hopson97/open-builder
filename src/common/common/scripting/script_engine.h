#pragma once

#include <iostream>
#include <optional>
#include <sol/sol.hpp>

struct ScriptEngine {
    ScriptEngine();

    bool runString(const std::string &script);
    bool runFile(const std::string &path);

    sol::function getLuaFunction(const char *functionName);

    template <typename Return, typename... Args>
    std::optional<Return> runLuaFunctionSafe(const char *functionName,
                                             Args &&... args)
    {
        sol::protected_function f = gameTable[functionName];
        sol::protected_function_result result = f(std::forward<Args>(args)...);

        if (result.valid()) {
            // Call succeeded
            Return x = result;
            return x;
        }
        else {
            sol::error err = result;
            std::string what = err.what();
            std::cout << "Error running function:" << functionName
                      << " Message: " << what << std::endl;
            return {};
        }
    }

    template <typename... Args>
    auto addTable(const std::string &tableName, Args &&... args)
    {
        return gameTable.create_named(tableName, std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    auto addType(const std::string &name, Args &&... args)
    {
        return lua.new_usertype<T>(name, std::forward<Args>(args)...);
    }

    sol::state lua;
    sol::table gameTable;
};