#pragma once

#include <glad/glad.h>
#include <type_traits>
#include <vector>

#include "gl_errors.h"

enum class GLType {
	UnsignedInt = GL_UNSIGNED_INT,
	Float = GL_FLOAT,
	Byte  = GL_BYTE,
};

struct VertexArray {
    GLuint handle = 0;
};

struct VertexArrayContainer {
    std::vector<GLuint> bufferObjects;
    VertexArray object;
    GLsizei indicesCount = 0;
};

enum class DrawStyle {
    Static = GL_STATIC_DRAW,
};

VertexArrayContainer createVertexArray();
void destroyVertexArray(VertexArrayContainer *vertexArray);


void bindVertexArray(VertexArray array);
void addIndexBuffer(VertexArrayContainer *vertexArray,
                    const std::vector<GLuint> &indices);

void drawElements(VertexArray array, GLsizei indices);

template <typename T>
void addVertexBuffer(VertexArrayContainer* container, int bufferMagntitude,
                     const std::vector<T> &bufferData,
                     DrawStyle bufferDrawStyle, GLType bufferType)
{
    bindVertexArray(container->object);

    GLuint vertexBuffer;
    glCheck(glGenBuffers(1, &vertexBuffer));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    glCheck(glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(T),
                         bufferData.data(),
                         static_cast<GLenum>(bufferDrawStyle)));

	glCheck(glVertexAttribPointer(
        container->bufferObjects.size(), bufferMagntitude,
                                  static_cast<GLenum>(bufferType),
                                  GL_FALSE, 0, (GLvoid *)0));

    glCheck(glEnableVertexAttribArray(container->bufferObjects.size()));
	container->bufferObjects.push_back(vertexBuffer);
}

