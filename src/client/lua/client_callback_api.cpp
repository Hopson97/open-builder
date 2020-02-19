#include "client_lua_api.h"

#include "client_lua_callback.h"
#include <common/scripting/script_engine.h>

void initClientCallbackApi(ScriptEngine& engine, ClientLuaCallbacks& callbacks)
{
    engine.addFunction("onClientStartup", [&callbacks](sol::function f) {
        callbacks.onClientStartup.push_back(f);
    });
}