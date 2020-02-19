#pragma once

#include <sol/sol.hpp>
#include <vector>

struct ClientLuaCallbacks {
    std::vector<sol::function> onClientStartup;
};

template <typename... Args>
void runClientStartupCallback(const std::vector<sol::function>& callbacks, Args&&... args)
{
    for (auto& callback : callbacks) {
        callback(std::forward<Args>(args)...);
    }
}