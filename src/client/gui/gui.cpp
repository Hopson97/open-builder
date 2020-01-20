#include "gui.h"
#include "../gl/primitive.h"

Gui::Gui()
    : m_quad(makeQuadVertexArray(1.f, 1.f))
{
}

Gui::~Gui()
{
    m_quad.destroy();
}

void Gui::processKeypress(sf::Event e)
{
}

void Gui::processMouseEvent(sf::Event e)
{
}

void Gui::render()
{
    m_quad.getDrawable().bindAndDraw();
}
