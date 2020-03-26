#include "client_lua_api.h"

#include <common/lua/script_engine.h>

#include "../gui/gui_system.h"
#include "../renderer/gui_renderer.h"

namespace {
    void initGuiRenderApi(sol::table& guiTable, GuiRenderer& guiRenderer)
    {
        guiTable["getTexture"] = [&guiRenderer](const std::string& path) {
            return guiRenderer.getTexture(path);
        };
    }

    void initGuiChangeApi(sol::table& guiTable, gui::GuiSystem& guiSystem)
    {
        guiTable["change"] = sol::overload(
            [&guiSystem](const std::string guiId, const sol::table& data) {
                guiSystem.changeGui(guiId, data);
            },
            [&guiSystem](const std::string guiId) { guiSystem.changeGui(guiId, {}); });

        guiTable["push"] = sol::overload(
            [&guiSystem](const std::string guiId, const sol::table& data) {
                guiSystem.pushGui(guiId, data);
            },
            [&guiSystem](const std::string guiId) { guiSystem.pushGui(guiId, {}); });

        guiTable["pop"] = [&guiSystem] { guiSystem.popGui(); };
    }

    void initGuiAddApi(sol::table& guiTable, gui::GuiSystem& guiSystem)
    {
        guiTable["addGui"] = [&guiSystem](const sol::table& guiDefintion) {
            gui::OverlayDefinition overlay;

            overlay.id = guiDefintion["id"];
            overlay.create = guiDefintion["create"];

            guiSystem.addGuiDefintion(overlay);
        };
    }
} // namespace

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::GuiSystem& guiSystem,
                   GuiRenderer* guiRenderer)
{
    auto guiTable = scriptEngine.addTable("gui");

    initGuiChangeApi(guiTable, guiSystem);
    initGuiAddApi(guiTable, guiSystem);

    if (guiRenderer) {
        initGuiRenderApi(guiTable, *guiRenderer);
    }
}
