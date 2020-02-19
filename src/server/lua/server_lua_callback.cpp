#include "server_lua_callback.h"

#include <common/scripting/script_engine.h>

void ServerLuaCallbacks::runPlayerJoinCallbacks()
{
    runLuaCallbacks(onPlayerJoinCallbacks);
}