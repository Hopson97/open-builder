#include "VertexArray.h"

#include "GLDebug.h"
#include <cassert>
#include <iostream>

namespace glpp {
    VertexArray::VertexArray()
    {
        glCheck(glGenVertexArrays(1, &m_vertexArray));
    }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
    {
        m_vertexArray = other.m_vertexArray;
        m_vertexBuffers = std::move(other.m_vertexBuffers);
        m_indicesCount = other.m_indicesCount;

        other.m_vertexArray = 0;
        other.m_indicesCount = 0;

        return *this;
    }

    VertexArray::VertexArray(VertexArray&& other) noexcept
        : m_vertexBuffers{std::move(other.m_vertexBuffers)}
        , m_vertexArray{other.m_vertexArray}
        , m_indicesCount{other.m_indicesCount}
    {
        other.m_vertexArray = 0;
        other.m_indicesCount = 0;
    }

    VertexArray::~VertexArray()
    {
        if (m_vertexArray) {
            glCheck(glDeleteVertexArrays(1, &m_vertexArray));
            glCheck(glDeleteBuffers(m_vertexBuffers.size(), m_vertexBuffers.data()));
        }
    }

    Drawable VertexArray::getDrawable() const
    {
        return {m_vertexArray, m_indicesCount};
    }

    void VertexArray::bind() const
    {
        glCheck(glBindVertexArray(m_vertexArray));
    }

    GLsizei VertexArray::indicesCount() const
    {
        return m_indicesCount;
    }

    void VertexArray::addAttribute(const std::vector<GLfloat>& data, int nPerVertex)
    {
        GLuint vbo;
        glCheck(glGenBuffers(1, &vbo));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        glCheck(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(),
                             GL_STATIC_DRAW));
        glCheck(glVertexAttribPointer(m_vertexBuffers.size(), nPerVertex, GL_FLOAT,
                                      GL_FALSE, 0, (GLvoid*)0));
        glCheck(glEnableVertexAttribArray(m_vertexBuffers.size()));
        m_vertexBuffers.push_back(vbo);
    }

    // void VertexArray::addAttribute(const std::vector<GLuint>& data, int nPerVertex)
    //{
    //    GLuint vbo;
    //    glCheck(glGenBuffers(1, &vbo));
    //    glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    //    glCheck(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]),
    //    data.data(),
    //                         GL_STATIC_DRAW));
    //    glCheck(glVertexAttribIPointer(m_vertexBuffers.size(), nPerVertex,
    //                                   GL_UNSIGNED_INT, 0, (GLvoid*)0));
    //    glCheck(glEnableVertexAttribArray(m_vertexBuffers.size()));
    //    m_vertexBuffers.push_back(vbo);
    //}

    void VertexArray::addElements(const std::vector<GLuint>& indices)
    {
        GLuint elementVbo;
        glCheck(glGenBuffers(1, &elementVbo));
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVbo));
        glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
                             indices.data(), GL_STATIC_DRAW));
        m_vertexBuffers.push_back(elementVbo);
        m_indicesCount = indices.size();
    }

    Drawable::Drawable(GLuint vao, GLsizei indices)
        : vertexArray{vao}
        , indicesCount{indices}
    {
    }

    void Drawable::draw(DrawMode mode) const
    {
        glCheck(glDrawElements(mode, indicesCount, GL_UNSIGNED_INT, nullptr));
    }

    void Drawable::bind() const
    {
        assert(vertexArray != 0);
        glCheck(glBindVertexArray(vertexArray));
    }

    void setPolygonMode(PolygonMode mode)
    {
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, mode));
    }

} // namespace glpp