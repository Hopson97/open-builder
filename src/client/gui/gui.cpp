#include "gui.h"
#include "../gl/primitive.h"

#include <iostream>

Gui::Gui()
    : m_quad(makeQuadVertexArray(1.f, 1.f))
{
    // GUI Shader
    m_guiShader.program.create("gui", "gui");
    m_guiShader.program.bind();
}

Gui::~Gui()
{
    m_quad.destroy();
    m_guiShader.program.destroy();

    for (auto &img : m_images) {
        img.m_image.destroy();
    }
}

void Gui::processKeypress(sf::Event e)
{
}

void Gui::processMouseEvent(sf::Event e)
{
}

void Gui::addImage(const std::string &string)
{
    GuiImage image;
    image.m_image.create(string);
    m_images.push_back(image);
    std::cout << "Added " << string << std::endl;
}

void Gui::render()
{
    m_guiShader.program.bind();
    auto d = m_quad.getDrawable();
    d.bind();
    for (auto &img : m_images) {
        img.m_image.bind();
        d.draw();
    }
}
