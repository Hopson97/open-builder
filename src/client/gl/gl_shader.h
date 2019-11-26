#pragma once

#include <glad/glad.h>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Shader {
    GLuint program = 0;
};

struct UniformLocation {
    GLuint ptr = 0;
};

Shader createShader(const std::string &vertexFile,
                    const std::string &fragmentFile);
void destroyShader(Shader *shader);
void useShader(Shader shader);

UniformLocation getUniformLocation(Shader shader, const char *name);
void loadUniform(UniformLocation location, const glm::vec3 &vector);
void loadUniform(UniformLocation location, const glm::mat4 &matrix);