#include <catch2/catch.hpp>

#include <client/gui/overlay.h>
#include <client/lua/client_lua_api.h>
#include <common/scripting/script_engine.h>

const std::string guiCreateScript = R"(

    function create(overlay)

    end

    game.gui.addGui{
        id = "test_gui",
        title = "Test GUI",
        create = create
    }
)";

const std::string guiAddScript = R"(
    game.gui.push("test_gui")
)";

TEST_CASE("GUI API Tests")
{
    ScriptEngine scriptEngine;
    gui::OverlayFactory overlayFactory;
    gui::OverlayStack overlayStack;
    luaInitGuiApi(scriptEngine, overlayFactory, overlayStack);

    SECTION("GUIs can be registered to a GUI factory")
    {
        scriptEngine.runLuaString(guiCreateScript);
        auto overlay = overlayFactory.createOverlay("test_gui");
        REQUIRE(overlay->definition.id == "test_gui");
        REQUIRE(overlay->definition.title == "Test GUI");
    }

    SECTION("GUIs can be shown using the API")
    {
        scriptEngine.runLuaString(guiCreateScript);
        scriptEngine.runLuaString(guiAddScript);

        overlayStack.update();

        REQUIRE(overlayStack.overlays.size() == 1);
    }
}