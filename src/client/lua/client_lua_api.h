#pragma once

class ScriptEngine;

#include <optional>

namespace gui {
class OverlayFactory;
struct OverlayStack;
} // namespace gui

class GuiRenderer;

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory,
                   gui::OverlayStack& overlayStack, GuiRenderer* guiRenderer = nullptr);