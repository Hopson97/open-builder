#include "server_lua_callback.h"

#include <common/lua/script_engine.h>

ServerLuaCallbacks::ServerLuaCallbacks(ScriptEngine& scriptEngine)
{
    scriptEngine.addCallbackApi("onPlayerJoin", onPlayerJoinCallbacks);
    scriptEngine.addCallbackApi("onPlayerLeave", onPlayerLeaveCallbacks);
}

void ServerLuaCallbacks::runPlayerJoinCallbacks()
{
    runLuaCallbacks(onPlayerJoinCallbacks);
}

void ServerLuaCallbacks::runPlayerLeaveCallbacks()
{
    runLuaCallbacks(onPlayerLeaveCallbacks);
}
