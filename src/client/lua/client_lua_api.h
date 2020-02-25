#pragma once

struct ScriptEngine;

#include <optional>

namespace gui {
class OverlayFactory;
class OverlayStack;
} // namespace gui

class GuiRenderer;

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory,
                   gui::OverlayStack& overlayStack, GuiRenderer* guiRenderer = nullptr);

void luaInitGuiWidgetApi(ScriptEngine& scriptEngine);