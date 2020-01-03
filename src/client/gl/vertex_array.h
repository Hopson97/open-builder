#pragma once

#include <glad/glad.h>
#include <vector>

namespace gl {

/**
 * @brief Minimal information for drawing with glDrawElements
 *
 */
class Drawable final {
  public:
    Drawable(GLuint vao, GLsizei indices);

    void bindAndDraw() const;

    void bind() const;
    void draw() const;

  private:
    const GLuint m_handle = 0;
    const GLsizei m_indicesCount = 0;
};

/**
 * @brief Wrapper for an OpenGL vertex array object (aka VAO)
 */
class VertexArray final {
  public:
    void create();
    void destroy();
    void bind() const;

    Drawable getDrawable() const;

    void addVertexBuffer(int magnitude, const std::vector<GLuint> &data);
    void addVertexBuffer(int magnitude, const std::vector<GLfloat> &data);
    void addIndexBuffer(const std::vector<GLuint> &indices);

    // private:
    std::vector<GLuint> m_bufferObjects;
    GLuint m_handle = 0;
    GLsizei m_indicesCount = 0;
};
} // namespace gl
