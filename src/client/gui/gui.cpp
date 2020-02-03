#include "gui.h"
#include "../gl/primitive.h"
#include "../gl/shader.h"
#include "../maths.h"
#include "text.h"
#include <common/debug.h>

#include <iostream>

//
//  Gui Dimensions
//
GuiDimension::GuiDimension(float xScale, float xOffset, float yScale, float yOffset)
    : scale{xScale, yScale}
    , offset{xOffset, yOffset}
{
}

glm::vec2 GuiDimension::apply(float width, float height)
{
    glm::vec2 vector;
    vector.x = scale.x * width * 100.0f;
    vector.y = scale.y * height * 100.0f;
    return vector + offset;
}

//
//  GUI Image
//

void GuiImage::setSource(const std::string& imageSource)
{
    if (texture.textureExists()) {
        texture.destroy();
    }
    texture.create(imageSource);
}

void GuiImage::setColour(float r, float g, float b)
{
    colour = {r, g, b};
}

void GuiImage::setPixelSize(float width, float height)
{
    size.offset = {width, height};
}

void GuiImage::setScaledSize(float width, float height)
{
    size.scale = {width, height};
}

void GuiImage::setScaledPosition(float x, float y)
{
    position.scale = {x, y};
}

void GuiImage::setPixelOffset(float x, float y)
{
    position.offset = {x, y};
}

Gui::Gui(float windowWidth, float windowHeight)
    : m_quad(makeQuadVertexArray(1.f, 1.f))
{
    // GUI Shader
    m_guiShader.program.create("gui", "gui");
    m_guiShader.program.bind();
    m_guiShader.modelLocation = m_guiShader.program.getUniformLocation("modelMatrix");
    m_guiShader.colorLocation = m_guiShader.program.getUniformLocation("colour");

    m_orthoMatrix = glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, -1.0f, 1.0f);

    m_guiShader.projectionLocation =
        m_guiShader.program.getUniformLocation("projectionMatrix");

    gl::loadUniform(m_guiShader.projectionLocation, m_orthoMatrix);
}

void Gui::addImage(sol::userdata image)
{
    if (!image.is<GuiImage>()) {
        std::cout << "Invalid type\n";
        return;
    }
    m_images.push_back(image);
}

void Gui::render(float width, float height)
{
    width /= 100.0f;
    height /= 100.0f;
    m_guiShader.program.bind();
    auto d = m_quad.getDrawable();
    d.bind();

    for (auto& image : m_images) {
        auto& img = image.as<GuiImage>();
        img.texture.bind();

        glm::vec2 pos = img.position.apply(width, height);
        glm::vec2 size = img.size.apply(width, height);

        glm::mat4 modelMatrix{1.0f};
        modelMatrix = glm::translate(modelMatrix, {pos.x, pos.y, 0.0f});
        modelMatrix = glm::scale(modelMatrix, {size.x, size.y, 0.0f});

        gl::loadUniform(m_guiShader.projectionLocation, m_orthoMatrix);
        gl::loadUniform(m_guiShader.modelLocation, modelMatrix);
        gl::loadUniform(m_guiShader.colorLocation, img.colour);

        d.draw();
    }
}

void Gui::renderText(Text& text)
{
    m_guiShader.program.bind();
    text.render(m_guiShader.modelLocation);
}

sol::table createGuiApi(ScriptEngine& engine)
{
    auto gui = engine.addTable("GUI");
    auto gdim = gui.new_usertype<GuiDimension>(
        "GDim",
        sol::constructors<GuiDimension(), GuiDimension(float, float, float, float)>());

    auto guiImage = gui.new_usertype<GuiImage>("Image");
    guiImage["setSource"] = &GuiImage::setSource;
    guiImage["setPixelSize"] = &GuiImage::setPixelSize;
    guiImage["setScaledSize"] = &GuiImage::setScaledSize;
    guiImage["setPixelOffset"] = &GuiImage::setPixelOffset;
    guiImage["setScaledPosition"] = &GuiImage::setScaledPosition;
    guiImage["setColor"] = &GuiImage::setColour;

    return gui;
}