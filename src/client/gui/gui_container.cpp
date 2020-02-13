#include "gui_container.h"

int GuiContainer::uidCount = 0;

GuiContainer::GuiContainer()
    : m_uid(uidCount++)
{
}

void GuiContainer::hide()
{
}
void GuiContainer::show()
{
}

void GuiContainer::render()
{
}