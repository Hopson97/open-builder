#pragma once

#include <glad/glad.h>
#include <vector>

struct VertexArrayObject {
    std::vector<GLuint> buffers;
    GLuint handle;
    GLsizei indicesCount;
};

