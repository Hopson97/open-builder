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
    m_guiShader.modelLocation =
        m_guiShader.program.getUniformLocation("modelMatrix");
    m_guiShader.colorLocation =
        m_guiShader.program.getUniformLocation("color3");
}


void Gui::addUsertypes(sol::table& gui_api)
{
    auto udim2_type = gui_api.new_usertype<GDim>(
        "GDim", sol::constructors<GDim(), GDim(float, float, float, float)>());

    auto color3_type = gui_api.new_usertype<Color3>(
        "Color3", sol::constructors<Color3(), Color3(float, float, float)>());
    color3_type["r"] = &Color3::r;
    color3_type["b"] = &Color3::g;
    color3_type["g"] = &Color3::b;

    auto image_type = gui_api.new_usertype<GuiImage>("Image");

    image_type["setSource"] = &GuiImage::setSource;
    image_type["setSize"] = &GuiImage::setSize;
    image_type["setPosition"] = &GuiImage::setPosition;
    image_type["setColor"] = &GuiImage::setColor;
}

void GuiImage::setSize(GDim new_size)
{
    m_size = new_size;
}

void GuiImage::setPosition(GDim new_pos)
{
    m_position = new_pos;
}

void GuiImage::setColor(Color3 new_color)
{
    m_color = new_color;
}

void Gui::processKeypress(sf::Event e)
{
}

void Gui::processMouseEvent(sf::Event e)
{
}

void Gui::addImage(sol::userdata image)
{
    if (!image.is<GuiImage>()) {
        std::cout << "Invalid type\n";
        return;
    }
    m_images.push_back(image);
}

void Gui::render(int width, int height)
{
    float pixel_width = 2.f / width;
    float pixel_height = 2.f / height;

    m_guiShader.program.bind();
    auto d = m_quad.getDrawable();
    d.bind();

    for (auto &g_img : m_images) {
        auto& img = g_img.as<GuiImage>();
        glm::mat4 modelMatrix{1.0f};
        modelMatrix = glm::translate(
            modelMatrix, glm::vec3(img.m_position.scale.x * 2 - 1 +
                                       img.m_position.offset.x * pixel_width,
                                   1 - img.m_position.scale.y * -2 - 2 +
                                       img.m_position.offset.y * pixel_height,
                                   0));

        modelMatrix = glm::scale(
            modelMatrix,
            glm::vec3(
                img.m_size.scale.x * 2 + img.m_size.offset.x * pixel_width,
                img.m_size.scale.y * 2 + img.m_size.offset.y * pixel_height,
                1));

        gl::loadUniform(m_guiShader.modelLocation, modelMatrix);
        gl::loadUniform(m_guiShader.colorLocation,
                        glm::vec3(img.m_color.r, img.m_color.g, img.m_color.b));
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
