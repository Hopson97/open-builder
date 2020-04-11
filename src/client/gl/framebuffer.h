#pragma once

#include <glad/glad.h>

#include "textures.h"

namespace gl {

    /**
     * @brief Wrapper for a OpenGL Framebuffer
     */
    class Framebuffer final {
      public:
        Framebuffer();
        ~Framebuffer();

        Framebuffer(Framebuffer&& other);
        Framebuffer& operator=(Framebuffer&& other);

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;

        void create(unsigned int width, unsigned int height);
        void destroy();

        void bind() const;

        void bindTexture() const;

      private:
        void reset();

        Texture2d m_texture;
        GLuint m_handle = 0;
        GLuint m_renderbufferHandle = 0;

        unsigned m_width = 0;
        unsigned m_height = 0;
    };

    void unbindFramebuffers(unsigned windowWidth, unsigned windowHeight);

} // namespace gl
