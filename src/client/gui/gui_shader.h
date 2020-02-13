#pragma once

#include "../gl/shader.h"

class GuiShader {

  public:
    GuiShader();

    void bind();

    void updateProjection(const glm::mat4& projectionMatrix);
    void updateTransform(const glm::mat4& modelMatrix);

  private:
    gl::Shader m_program;
    gl::UniformLocation m_projectionLocation;
    gl::UniformLocation m_modelLocation;
    gl::UniformLocation m_colorLocation;
};
