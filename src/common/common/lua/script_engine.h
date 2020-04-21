#pragma once

#include <iostream>
#include <optional>
#include <sol/sol.hpp>
#include <vector>

struct ScriptEngine {
    ScriptEngine();

    bool runLuaString(const std::string& script);
    bool runLuaFile(const std::string& path);

    void addCallbackApi(const char* name, std::vector<sol::function>& callbackList);

    /**
     * @brief Gets a Lua function
     *  This would be a function that already exists, usually by running the
     * .lua script files Bare in mind, it will only find functions that belong
     * to OpenBuilder (aka in 'game' lua global table)
     * @param functionName The name of the function to find
     * @return sol::function The Lua function
     */
    sol::function getLuaFunction(const char* functionName);

    /**
     * @brief Runs a Lua function in a safe manner
     * Like 'getLuaFunction', this would be a function in that belongs to
     * OpenBuilder, aka one in the 'game' global lua table
     * @tparam R Return type of the lua function
     * @tparam Args Arguments into the function
     * @param functionName
     * @param args
     * @return std::optional<R>
     */
    template <typename R, typename... Args>
    std::optional<R> runLuaFunctionSafe(const char* functionName, Args&&... args);

    template <typename... Args>
    auto addTable(const char* tableName, Args&&... args);

    template <typename T, typename... Args>
    auto addType(const char* name, Args&&... args);

    template <typename F>
    auto addFunction(const char* name, F function);

    sol::state lua;
    sol::table gameTable;

  private:
    /**
     * For example, random number generators etc
     * @brief Gives Lua some utility functionality
     * @param script The script engine with the Lua state
     */
    void luaInitUtilApi();
};

template <typename R, typename... Args>
std::optional<R> ScriptEngine::runLuaFunctionSafe(const char* functionName,
                                                  Args&&... args)
{
    sol::protected_function function = gameTable[functionName];
    sol::protected_function_result result = function(std::forward<Args>(args)...);
    if (result.valid()) {
        R res = result;
        return res;
    }
    else {
        sol::error err = result;
        std::cout << "Error running function:" << functionName
                  << " Message: " << err.what() << '\n';
        return {};
    }
}

template <typename T, typename... Args>
auto ScriptEngine::addType(const char* name, Args&&... args)
{
    return lua.new_usertype<T>(name, std::forward<Args>(args)...);
}

template <typename... Args>
auto ScriptEngine::addTable(const char* tableName, Args&&... args)
{
    return gameTable.create_named(tableName, std::forward<Args>(args)...);
}

template <typename F>
auto ScriptEngine::addFunction(const char* name, F function)
{
    return gameTable[name] = function;
}

/**
 * @brief Runs Lua callbacks
 * As in, functions defined in Lua that have registered as a callback
 * @tparam Args The arguments to the callback function
 * @param callbacks List of functions to be called
 * @param args The arguments to the callback function
 */
template <typename... Args>
void runLuaCallbacks(const std::vector<sol::function>& callbacks, Args&&... args)
{
    for (auto& callback : callbacks) {
        callback(std::forward<Args>(args)...);
    }
}
