#include "client_lua_api.h"

#include <common/lua/script_engine.h>

#include "../gui/gui_system.h"
#include "../renderer/gui_renderer.h"

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::GuiSystem& guiSystem,
                   GuiRenderer* guiRenderer)
{
    using Gui = gui::GuiSystem;

    scriptEngine.gameTable["gui"] = [&guiSystem]() -> gui::GuiSystem& {
        return guiSystem;
    };

    auto guiApi = scriptEngine.lua.new_usertype<Gui>("GUISystem");
    guiApi["push"] = sol::overload(
        &Gui::pushGui, [](Gui& gui, const std::string& id) { gui.pushGui(id, {}); });
    guiApi["change"] = sol::overload(
        &Gui::changeGui, [](Gui& gui, const std::string& id) { gui.changeGui(id, {}); });
    guiApi["pop"] = &Gui::popGui;

    guiApi["addGui"] = [](Gui& gui, const sol::table& guiDefintion) {
        gui::OverlayDefinition overlay;

        overlay.id = guiDefintion["id"];
        overlay.create = guiDefintion["create"];

        gui.addGuiDefintion(overlay);
    };

    if (guiRenderer) {
        scriptEngine.gameTable["getTexture"] = [guiRenderer](const std::string& path) {
            return guiRenderer->getTexture(path);
        };
    }
}
