#include "gui_shader.h"

GuiShader::GuiShader()
{
    m_program.create("gui", "gui");

    m_program.bind();
    m_modelLocation = m_program.getUniformLocation("modelMatrix");
    m_colourLocation = m_program.getUniformLocation("colour");
    m_projectionLocation = m_program.getUniformLocation("projectionMatrix");
}

void GuiShader::bind()
{
    m_program.bind();
}

void GuiShader::updateProjection(const glm::mat4& projectionMatrix)
{
    gl::loadUniform(m_projectionLocation, projectionMatrix);
}

void GuiShader::updateTransform(const glm::mat4& modelMatrix)
{
    gl::loadUniform(m_modelLocation, modelMatrix);
}

void GuiShader::updateColour(const glm::vec3& colour)
{
    gl::loadUniform(m_colourLocation, colour);
}