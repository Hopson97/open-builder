#include "client_lua_api.h"

#include "../gui/overlay.h"
#include "../gui/widget/button_widget.h"
#include "../gui/widget/image_widget.h"
#include "../gui/widget/label_widget.h"
#include <common/scripting/script_engine.h>

namespace {

template <typename T>
void addCommonAPI(sol::usertype<T>& api)
{
    api["position"] = sol::property(&T::setPosition);
    api["size"] = sol::property(&T::setSize);
    api["hide"] = &T::hide;
    api["show"] = &T::show;

    api["setColour"] = &T::setColour;
}

void addGuiDimensionApi(ScriptEngine& scriptEngine)
{
    auto gdimApi = scriptEngine.lua.new_usertype<gui::GuiDimension>(
        "GuiDim", sol::constructors<gui::GuiDimension(float, float, float, float)>());
}

void addGuiImageApi(ScriptEngine& engine)
{
    auto imageWidgetApi = engine.lua.new_usertype<gui::ImageWidget>("ImageWidget");
    imageWidgetApi["image"] = sol::property(&gui::ImageWidget::setImage);

    addCommonAPI(imageWidgetApi);
}

void addGuiLabelApi(ScriptEngine& engine)
{
    auto labelApi = engine.lua.new_usertype<gui::LabelWidget>("LabelWidget");
    labelApi["text"] = sol::property(&gui::LabelWidget::setText);
    labelApi["textSize"] = sol::property(&gui::LabelWidget::setTextSize);

    addCommonAPI(labelApi);
}

void addGuiButtonApi(ScriptEngine& engine)
{
    auto buttonApi = engine.lua.new_usertype<gui::ButtonWidget>("ButtonWidget");
    buttonApi["text"] = sol::property(&gui::ButtonWidget::setText);
    buttonApi["textSize"] = sol::property(&gui::ButtonWidget::setTextSize);

    buttonApi["image"] = sol::property(&gui::ButtonWidget::setImage);

    buttonApi["onClick"] = sol::property(&gui::ButtonWidget::setOnClick);
    buttonApi["onMouseOver"] = sol::property(&gui::ButtonWidget::setOnMouseOver);

    addCommonAPI(buttonApi);
}

void initGuiOverlayApi(ScriptEngine& engine)
{
    auto overlayApi = engine.lua.new_usertype<gui::Overlay>("GuiOverlay");
    overlayApi["addImage"] = &gui::Overlay::addImage;
    overlayApi["addLabel"] = &gui::Overlay::addLabel;
    overlayApi["addButton"] = &gui::Overlay::addButton;
}

} // namespace

void luaInitGuiWidgetApi(ScriptEngine& scriptEngine)
{
    initGuiOverlayApi(scriptEngine);

    addGuiDimensionApi(scriptEngine);
    addGuiImageApi(scriptEngine);
    addGuiLabelApi(scriptEngine);
    addGuiButtonApi(scriptEngine);
}
