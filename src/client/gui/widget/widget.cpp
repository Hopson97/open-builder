#include "widget.h"

#include "../component/component.h"

#include <iostream>

namespace gui {

    void Widget::setColour(float r, float g, float b)
    {
        for (auto component : componentList) {
            component->colour = {r, g, b};
        }
    }

    void Widget::setId(const std::string& newid)
    {
        id = newid;
        std::cout << "id set to " << id << std::endl;
    }

    void Widget::hide()
    {
        for (auto component : componentList) {
            component->hide();
        }
    }

    void Widget::show()
    {
        for (auto component : componentList) {
            component->show();
        }
    }
} // namespace gui
