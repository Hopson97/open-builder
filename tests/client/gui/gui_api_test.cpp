#include <catch2/catch.hpp>

#include <client/gui/overlay_factory.h>
#include <client/lua/client_lua_api.h>
#include <common/scripting/script_engine.h>

const char* guiCreateScript = R"(

    function create()
        print("Why")
    end

    game.gui.defineGui{
        id = "test_gui",
        title = "Test GUI",
        create = create
    }
)";

TEST_CASE("GUI API Tests")
{
    ScriptEngine scriptEngine;
    gui::OverlayFactory overlayFactory;
    luaInitGuiApi(scriptEngine, overlayFactory, nullptr);

    SECTION("GUIs can be registered to a GUI factory")
    {
        scriptEngine.runLuaString(guiCreateScript);
        auto overlay = overlayFactory.createOverlay("test_gui");
        REQUIRE(overlay->definition.id == "test_gui");
        REQUIRE(overlay->definition.title == "Test GUI");
    }
}