#pragma once

#include <array>
#include <glad/glad.h>
#include <string>

namespace client {

    class CubeTexture final {
      public:
        CubeTexture() = default;
        CubeTexture(const std::array<std::string, 6> &files);
        CubeTexture(const CubeTexture &other) = delete;
        CubeTexture &operator=(const CubeTexture &other) = delete;
        ~CubeTexture();

        /**
            MUST BE IN THIS ORDER:
                -right
                -left
                -top
                -bottom
                -back
                -front
        */
        void loadFromFiles(const std::array<std::string, 6> &files);

        void bind() const;

      private:
        GLuint m_texId;
    };

} // namespace client