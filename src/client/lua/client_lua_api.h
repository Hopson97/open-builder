#pragma once

class ScriptEngine;
class GuiMaster;
struct ClientLuaCallbacks;

void initGuiApi(ScriptEngine& scriptEngine, GuiMaster& guiMaster);
void initClientCallbackApi(ScriptEngine& scriptEngine, ClientLuaCallbacks& callbacks);