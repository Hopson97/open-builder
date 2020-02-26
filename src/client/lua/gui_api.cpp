#include "client_lua_api.h"

#include <common/scripting/script_engine.h>

#include "../gui/overlay.h"
#include "../renderer/gui_renderer.h"

namespace {
void initOverlayDefinitionCreateApi(sol::table& guiTable,
                                    gui::OverlayFactory& overlayFactory)
{
    guiTable["addGui"] = [&](const sol::table& guiDefintion) {
        gui::OverlayDefinition overlay;

        overlay.id = guiDefintion["id"];
        overlay.title = guiDefintion["title"];
        overlay.create = guiDefintion["create"];

        overlayFactory.addOverlay(overlay);
    };
}

void initGuiRenderApi(sol::table& guiTable, GuiRenderer& guiRenderer)
{
    guiTable["getTexture"] = [&guiRenderer](const std::string& path) {
        return guiRenderer.getTexture(path);
    };
}

void initGuiStackApi(sol::table& guiTable, gui::OverlayFactory& overlayFactory,
                     gui::OverlayStack& overlayStack)
{
    guiTable["push"] = sol::overload(
        [&](const std::string guiId, const std::string& data) {
            overlayStack.pushLayer(overlayFactory.createOverlay(guiId, data));
        },
        [&](const std::string guiId) {
            overlayStack.pushLayer(overlayFactory.createOverlay(guiId, ""));
        });

    guiTable["change"] = sol::overload(
        [&](const std::string guiId, const std::string& data) {
            overlayStack.resetToLayer(overlayFactory.createOverlay(guiId, data));
        },
        [&](const std::string guiId) {
            overlayStack.resetToLayer(overlayFactory.createOverlay(guiId, ""));
        });

    guiTable["pop"] = [&]() { overlayStack.popLayer(); };

} // namespace

} // namespace

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory,
                   gui::OverlayStack& overlayStack, GuiRenderer* guiRenderer)
{
    auto guiTable = scriptEngine.addTable("gui");
    initOverlayDefinitionCreateApi(guiTable, overlayFactory);
    initGuiStackApi(guiTable, overlayFactory, overlayStack);

    if (guiRenderer) {
        initGuiRenderApi(guiTable, *guiRenderer);
    }
}
