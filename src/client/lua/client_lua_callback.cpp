#include "client_lua_callback.h"

#include <common/scripting/script_engine.h>

ClientLuaCallbacks::ClientLuaCallbacks(ScriptEngine& scriptEngine)
{
    scriptEngine.addCallbackApi("onClientStartup", onClientStartup);
}

void ClientLuaCallbacks::runClientStartupCallbacks()
{
    runLuaCallbacks(onClientStartup);
}