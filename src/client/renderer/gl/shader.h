#pragma once

#include <glad/glad.h>
#include <string>

#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace client {

    class Shader final {
      public:
        Shader() = default;
        Shader(const std::string &vertexFile, const std::string &fragmentFile);

        Shader(Shader &&other);
        Shader &operator=(Shader &&other);

        Shader(const Shader &other) = delete;
        Shader &operator=(const Shader &other) = delete;

        ~Shader();

        void load(const std::string &vertexFile,
                  const std::string &fragmentFile);

        GLuint getUniformLocation(const char *name) const;

        void use() const;

      private:
        GLuint m_program = 0;
    };

    void loadVector3(GLuint location, const glm::vec3 &vector);
    void loadMatrix4(GLuint location, const glm::mat4 &matrix);
} // namespace client