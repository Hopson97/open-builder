#pragma once

#include <sol/sol.hpp>
#include <vector>

struct ClientLuaCallbacks {
    std::vector<sol::function> onClientStartup;

    void runClientStartupCallbacks();
};
