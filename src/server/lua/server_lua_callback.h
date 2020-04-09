#pragma once

#include <sol/sol.hpp>
#include <vector>

struct ScriptEngine;

class ServerLuaCallbacks {
  public:
    ServerLuaCallbacks(ScriptEngine& engine);

    // @TODO The player entity would be passed into this
    void runPlayerJoinCallbacks();
    void runPlayerLeaveCallbacks();

  private:
    std::vector<sol::function> onPlayerJoinCallbacks;
    std::vector<sol::function> onPlayerLeaveCallbacks;
};
