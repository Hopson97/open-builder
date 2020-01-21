#include "gui.h"
#include "../gl/primitive.h"
#include "../gl/shader.h"
#include "../maths.h"

#include <iostream>

Gui::Gui()
    : m_quad(makeQuadVertexArray(1.f, 1.f))
{
    // GUI Shader
    m_guiShader.program.create("gui", "gui");
    m_guiShader.program.bind();
    m_guiShader.modelLocation = m_guiShader.program.getUniformLocation("modelMatrix");
}

Gui::~Gui()
{
    m_quad.destroy();
    m_guiShader.program.destroy();

    for (auto &img : m_images) {
        img.m_image.destroy();
    }
}

void Gui::addUsertypes(sol::table& gui_api)
{
    auto udim2_type = gui_api.new_usertype<GDim>("GDim",
        sol::constructors < GDim(), GDim(float, float, float, float)>());

    auto image_type = gui_api.new_usertype<GuiImage>("Image");

    image_type["setSource"] = &GuiImage::setSource;
    image_type["setSize"] = &GuiImage::setSize;
    image_type["setPosition"] = &GuiImage::setPosition;
}

void GuiImage::setSize(GDim new_size)
{
    m_size = new_size;
}

void GuiImage::setPosition(GDim new_pos)
{
    m_position = new_pos;
}

void Gui::processKeypress(sf::Event e)
{
}

void Gui::processMouseEvent(sf::Event e)
{
}

void Gui::addImage(const GuiImage &image)
{
    m_images.push_back(image);
}

void Gui::render()
{
    m_guiShader.program.bind();
    auto d = m_quad.getDrawable();
    d.bind();
    for (auto &img : m_images) {
        glm::mat4 modelMatrix{ 1.0f };
        modelMatrix = glm::translate(modelMatrix, glm::vec3(img.m_position.scale.x*2 - 1, 1 - img.m_position.scale.y*-2 - 2, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(img.m_size.scale.x*2, img.m_size.scale.y*2, 1));

        gl::loadUniform(m_guiShader.modelLocation, modelMatrix);
        img.m_image.bind();
        d.draw();
    }
}


void GuiImage::setSource(const std::string &imageSource)
{
    if (m_image.textureExists()) {
        m_image.destroy();
    }
    m_image.create(imageSource);
}
