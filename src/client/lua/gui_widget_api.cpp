#include "client_lua_api.h"

#include "../gui/overlay.h"
#include "../gui/widget/button_widget.h"
#include "../gui/widget/checkbox_widget.h"
#include "../gui/widget/image_widget.h"
#include "../gui/widget/label_widget.h"
#include "../gui/widget/text_box_widget.h"
#include <common/lua/script_engine.h>

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

    template <typename T>
    void addCommonLabelApi(sol::usertype<T>& api)
    {
        api["text"] = sol::property(&T::setText);
        api["textSize"] = sol::property(&T::setTextSize);
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
        addCommonLabelApi(labelApi);
        addCommonAPI(labelApi);
    }

    void addGuiCenteredLabelApi(ScriptEngine& engine)
    {
        auto centerLabelApi =
            engine.lua.new_usertype<gui::CenteredLabelWidget>("CenteredLabelWidget");
        addCommonLabelApi(centerLabelApi);
        addCommonAPI(centerLabelApi);
    }

    void addGuiButtonApi(ScriptEngine& engine)
    {
        auto buttonApi = engine.lua.new_usertype<gui::ButtonWidget>("ButtonWidget");
        buttonApi["image"] = sol::property(&gui::ButtonWidget::setImage);

        buttonApi["onClick"] = sol::property(&gui::ButtonWidget::setOnClick);
        buttonApi["onMouseOver"] = sol::property(&gui::ButtonWidget::setOnMouseOver);
        buttonApi["onMouseOff"] = sol::property(&gui::ButtonWidget::setOnMouseOff);

        addCommonLabelApi(buttonApi);
        addCommonAPI(buttonApi);
    }

    void addGuiCheckBoxApi(ScriptEngine& engine)
    {
        auto checkBoxApi = engine.lua.new_usertype<gui::CheckBoxWidget>("CheckBoxWidget");
        checkBoxApi["checkedImage"] =
            sol::property(&gui::CheckBoxWidget::setCheckedImage);
        checkBoxApi["uncheckedImage"] =
            sol::property(&gui::CheckBoxWidget::setUncheckedImage);
        checkBoxApi["image"] = sol::property(&gui::CheckBoxWidget::setImage);
        checkBoxApi["checked"] = sol::property(&gui::CheckBoxWidget::getChecked,
                                               &gui::CheckBoxWidget::setChecked);

        checkBoxApi["onMouseOver"] = sol::property(&gui::CheckBoxWidget::setOnMouseOver);
        checkBoxApi["onMouseOff"] = sol::property(&gui::CheckBoxWidget::setOnMouseOff);

        addCommonLabelApi(checkBoxApi);
        addCommonAPI(checkBoxApi);
    }

    void addGuiTextboxApi(ScriptEngine& engine)
    {
        auto textboxApi = engine.lua.new_usertype<gui::TextBoxWidget>("TextBoxWidget");
        textboxApi["image"] = sol::property(&gui::TextBoxWidget::setImage);

        textboxApi["onMouseOver"] = sol::property(&gui::TextBoxWidget::setOnMouseOver);
        textboxApi["onMouseOff"] = sol::property(&gui::TextBoxWidget::setOnMouseOff);

        textboxApi["getText"] = &gui::TextBoxWidget::getText;
        textboxApi["limitChars"] = &gui::TextBoxWidget::limitChars;
        textboxApi["allowAllChars"] = &gui::TextBoxWidget::allowAllChars;
        textboxApi["hideInput"] = &gui::TextBoxWidget::hideInputText;
        textboxApi["placeholder"] = sol::property(&gui::TextBoxWidget::setPlaceholder);
        textboxApi["maxLength"] = sol::property(&gui::TextBoxWidget::setMaxLength);
        textboxApi["label"] = sol::property(&gui::TextBoxWidget::setLabelText);

        addCommonLabelApi(textboxApi);
        addCommonAPI(textboxApi);
    }

    void initGuiOverlayApi(ScriptEngine& engine)
    {
        auto overlayApi = engine.lua.new_usertype<gui::Overlay>("GuiOverlay");
        overlayApi["addImage"] = &gui::Overlay::addImage;
        overlayApi["addLabel"] = &gui::Overlay::addLabel;
        overlayApi["addButton"] = &gui::Overlay::addButton;
        overlayApi["addCenteredLabel"] = &gui::Overlay::addCenteredLabel;
        overlayApi["addTextBox"] = &gui::Overlay::addTextBox;
        overlayApi["addCheckBox"] = &gui::Overlay::addCheckBox;

        overlayApi["hide"] = &gui::Overlay::hide;
        overlayApi["show"] = &gui::Overlay::show;
    }

} // namespace

void luaInitGuiWidgetApi(ScriptEngine& scriptEngine)
{
    initGuiOverlayApi(scriptEngine);

    addGuiDimensionApi(scriptEngine);
    addGuiImageApi(scriptEngine);
    addGuiLabelApi(scriptEngine);
    addGuiButtonApi(scriptEngine);
    addGuiTextboxApi(scriptEngine);
    addGuiCenteredLabelApi(scriptEngine);
    addGuiCheckBoxApi(scriptEngine);
}
