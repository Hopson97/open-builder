#pragma once

class ScriptEngine;

namespace gui {
class OverlayFactory;
}

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory);