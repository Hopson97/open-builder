#include "server_lua_api.h"

#include "server_lua_callback.h"
#include <common/scripting/script_engine.h>

void luaInitServerCallbackApi(ScriptEngine& script, ServerLuaCallbacks& callbacks)
{
    script.addFunction("onPlayerJoin", [&callbacks](sol::function f) {
        callbacks.onPlayerJoinCallbacks.push_back(f);
    });
}