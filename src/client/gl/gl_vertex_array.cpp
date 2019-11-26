#include "gl_errors.h"
#include "gl_object.h"

namespace gl {
Drawable::Drawable(GLuint vao, GLsizei indices)
    : m_handle(vao)
    , m_indicesCount(indices)
{
}

void Drawable::drawElements() const
{
    glCheck(glBindVertexArray(m_handle));
    glCheck(
        glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, nullptr));
}

void VertexArray::create()
{
    glCheck(glGenVertexArrays(1, &m_handle));
}

void VertexArray::destroy()
{
    glCheck(glDeleteVertexArrays(1, &m_handle));
    glCheck(glDeleteBuffers(m_bufferObjects.size(), m_bufferObjects.data()));
    m_bufferObjects.clear();
    m_handle = 0;
    m_indicesCount = 0;
}

void VertexArray::bind() const
{
    glCheck(glBindVertexArray(m_handle));
}

Drawable VertexArray::getDrawable() const
{
    return {m_handle, m_indicesCount};
}

void VertexArray::addVertexBuffer(int magnitude,
                                  const std::vector<GLfloat> &data)
{
    bind();

    GLuint vertexBuffer;
    glCheck(glGenBuffers(1, &vertexBuffer));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    glCheck(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
                         data.data(), GL_STATIC_DRAW));

    glCheck(glVertexAttribPointer(m_bufferObjects.size(), magnitude, GL_FLOAT,
                                  GL_FALSE, 0, (GLvoid *)0));

    glCheck(glEnableVertexAttribArray(m_bufferObjects.size()));

    m_bufferObjects.push_back(vertexBuffer);
}

void VertexArray::addIndexBuffer(const std::vector<GLuint> &indices)
{
    bind();

    GLuint elementBuffer;
    glCheck(glGenBuffers(1, &elementBuffer));
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer));

    glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         indices.size() * sizeof(GLuint), indices.data(),
                         GL_STATIC_DRAW));

    m_bufferObjects.push_back(elementBuffer);
    m_indicesCount = indices.size();
}

} // namespace gl