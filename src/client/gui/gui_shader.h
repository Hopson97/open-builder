#pragma once

#include "../gl/shader.h"

class GuiShader final {

  public:
    GuiShader();

    void bind();

    void updateProjection(const glm::mat4& projectionMatrix);
    void updateTransform(const glm::mat4& modelMatrix);
    void updateColour(const glm::vec3& colour);

  private:
    gl::Shader m_program;
    gl::UniformLocation m_projectionLocation;
    gl::UniformLocation m_modelLocation;
    gl::UniformLocation m_colourLocation;
};
