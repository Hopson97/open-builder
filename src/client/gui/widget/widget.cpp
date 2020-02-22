#include "widget.h"

#include "../component/component.h"

namespace gui {
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
