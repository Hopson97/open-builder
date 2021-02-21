#pragma once

#include "Texture.h"
#include <vector>

namespace glpp {
    class Framebuffer {
      public:
        Framebuffer(GLuint width, GLuint height);
        Framebuffer& operator=(Framebuffer&& other) noexcept;
        Framebuffer(Framebuffer&& other) noexcept;
        ~Framebuffer();

        void attachTexture();
        void finalise();

        void bind() const;
        void bindTexture(unsigned index) const;
        static void unbind(GLuint width, GLuint height);

        Framebuffer& operator=(Framebuffer& other) = delete;
        Framebuffer(Framebuffer& other) = delete;

      private:
        std::vector<Texture2d> m_textureAttachments;
        GLuint m_width = 0;
        GLuint m_height = 0;
        GLuint m_renderbuffer = 0;
        GLuint m_framebuffer = 0;
    };
} // namespace glpp