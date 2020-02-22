#include "client_lua_api.h"

#include "../gui/widget.h"
#include <common/scripting/script_engine.h>

namespace 
{
    void addGuiImageApi(ScriptEngine& engine)
    {
        auto imageWidgetApi = engine.lua.new_usertype<gui::ImageWidget>("ImageWidget");
        imageWidgetApi["position"] = sol::property(&gui::ImageWidget::setPosition);
        imageWidgetApi["size"] = sol::property(&gui::ImageWidget::setSize);

    }
} // namespace 


void luaInitGuiWidgetApi(ScriptEngine& scriptEngine)
{
    addGuiImageApi(scriptEngine);
}
