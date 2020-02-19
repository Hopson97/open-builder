#pragma once

#include <sol/sol.hpp>
#include <vector>

struct ServerLuaCallbacks {
    std::vector<sol::function> onPlayerJoinCallbacks;

    // TODO The player entity would be passed into this
    void runPlayerJoinCallbacks();
};
