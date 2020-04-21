#include "vertex_array.h"
#include "gl_errors.h"
#include <iostream>

namespace {
    GLuint genVbo()
    {
        GLuint vertexBuffer;
        glCheck(glGenBuffers(1, &vertexBuffer));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
        return vertexBuffer;
    }

    template <typename T>
    void bufferData(const std::vector<T>& data)
    {
        glCheck(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(),
                             GL_STATIC_DRAW));
    }

    void vertexAttribPointer(GLuint index, GLint mag, GLenum type)
    {
        glCheck(glVertexAttribPointer(index, mag, type, GL_FALSE, 0, (GLvoid*)0));
    }

    void vertexAttribIntPointer(GLuint index, GLint mag, GLenum type)
    {
        glCheck(glVertexAttribIPointer(index, mag, type, 0, (GLvoid*)0));
    }
} // namespace

namespace gl {
    Drawable::Drawable(GLuint vao, GLsizei indices)
        : m_handle(vao)
        , m_indicesCount(indices)
    {
    }

    void Drawable::bindAndDraw(GLenum drawMode) const
    {
        bind();
        draw(drawMode);
    }

    void Drawable::bind() const
    {
        glCheck(glBindVertexArray(m_handle));
    }

    void Drawable::draw(GLenum drawMode) const
    {
        glCheck(glDrawElements(drawMode, m_indicesCount, GL_UNSIGNED_INT, nullptr));
    }

    //
    //  Vertex array
    //
    VertexArray::VertexArray()
    {
        glCheck(glGenVertexArrays(1, &m_handle));
    }

    VertexArray::~VertexArray()
    {
        destroy();
    }

    VertexArray::VertexArray(VertexArray&& other)
    {
        *this = std::move(other);
    }

    VertexArray& VertexArray::operator=(VertexArray&& other)
    {
        destroy();
        m_bufferObjects = std::move(other.m_bufferObjects);
        m_handle = other.m_handle;
        m_indicesCount = other.m_indicesCount;
        other.reset();
        return *this;
    }

    void VertexArray::create()
    {
        if (!m_handle) {
            glCheck(glGenVertexArrays(1, &m_handle));
        }
    }

    void VertexArray::destroy()
    {
        glCheck(glDeleteVertexArrays(1, &m_handle));
        glCheck(glDeleteBuffers(m_bufferObjects.size(), m_bufferObjects.data()));
        reset();
    }

    void VertexArray::bind() const
    {
        glCheck(glBindVertexArray(m_handle));
    }

    Drawable VertexArray::getDrawable() const
    {
        return {m_handle, m_indicesCount};
    }

    void VertexArray::addVertexBuffer(int magnitude, const std::vector<GLuint>& data)
    {
        bind();
        GLuint vertexBuffer = genVbo();
        bufferData(data);
        vertexAttribIntPointer(m_bufferObjects.size(), magnitude, GL_UNSIGNED_INT);
        glCheck(glEnableVertexAttribArray(m_bufferObjects.size()));
        m_bufferObjects.push_back(vertexBuffer);
    }

    void VertexArray::addVertexBuffer(int magnitude, const std::vector<GLfloat>& data)
    {
        bind();
        GLuint vertexBuffer = genVbo();
        bufferData(data);
        vertexAttribPointer(m_bufferObjects.size(), magnitude, GL_FLOAT);
        glCheck(glEnableVertexAttribArray(m_bufferObjects.size()));
        m_bufferObjects.push_back(vertexBuffer);
    }

    void VertexArray::addIndexBuffer(const std::vector<GLuint>& indices)
    {
        bind();

        GLuint elementBuffer;
        glCheck(glGenBuffers(1, &elementBuffer));
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer));

        glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                             indices.data(), GL_STATIC_DRAW));

        m_bufferObjects.push_back(elementBuffer);
        m_indicesCount = indices.size();
    }

    void VertexArray::reset()
    {
        m_bufferObjects.clear();
        m_handle = 0;
        m_indicesCount = 0;
    }

} // namespace gl
