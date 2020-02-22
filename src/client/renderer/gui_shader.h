#pragma once

#include "../gl/shader.h"

class GuiShader final {

  public:
    GuiShader()
    {
        m_program.create("gui", "gui");

        m_program.bind();
        m_modelLocation = m_program.getUniformLocation("modelMatrix");
        m_colourLocation = m_program.getUniformLocation("colour");
        m_projectionLocation = m_program.getUniformLocation("projectionMatrix");
    }
    void bind()
    {
        m_program.bind();
    }

    void updateProjection(const glm::mat4& projectionMatrix)
    {
        gl::loadUniform(m_projectionLocation, projectionMatrix);
    }
    void updateTransform(const glm::mat4& modelMatrix)
    {
        gl::loadUniform(m_modelLocation, modelMatrix);
    }
    void updateColour(const glm::vec3& colour)
    {
        gl::loadUniform(m_colourLocation, colour);
    }

  private:
    gl::Shader m_program;
    gl::UniformLocation m_projectionLocation;
    gl::UniformLocation m_modelLocation;
    gl::UniformLocation m_colourLocation;
};
