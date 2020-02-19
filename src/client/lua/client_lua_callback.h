#pragma once

#include <sol/sol.hpp>
#include <vector>

class ScriptEngine;

class ClientLuaCallbacks {
  public:
    ClientLuaCallbacks(ScriptEngine& scriptEngine);

    void runClientStartupCallbacks();

  private:
    std::vector<sol::function> onClientStartup;
};
