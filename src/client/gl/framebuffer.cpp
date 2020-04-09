#include "framebuffer.h"

#include "gl_errors.h"
#include <utility>

namespace gl {
    Framebuffer::Framebuffer()
    {
        glCheck(glGenFramebuffers(1, &m_handle));
    }

    Framebuffer::~Framebuffer()
    {
        destroy();
    }

    Framebuffer::Framebuffer(Framebuffer&& other)
    {
        *this = std::move(other);
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other)
    {
        destroy();
        m_texture = std::move(other.m_texture);
        m_handle = other.m_handle;
        m_renderbufferHandle = other.m_renderbufferHandle;
        other.reset();
        return *this;
    }

    void Framebuffer::create(unsigned int width, unsigned int height)
    {
        if (!m_handle) {
            glCheck(glGenFramebuffers(1, &m_handle));
        }
        m_width = width;
        m_height = height;

        bind();
        GLuint textureHandle = m_texture.createFramebufferTexture(m_width, m_height);
        glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_2D, textureHandle, 0));

        // clang-format off
    glCheck(glGenRenderbuffers(1, &m_renderbufferHandle));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferHandle));
    glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height));
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferHandle));
        // clang-format on

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Renderbuffer failed to be created");
        }
    }

    void Framebuffer::destroy()
    {
        glCheck(glDeleteFramebuffers(1, &m_handle));
        if (m_renderbufferHandle) {
            glCheck(glDeleteRenderbuffers(1, &m_renderbufferHandle));
        }
    }

    void Framebuffer::bind() const
    {
        glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_handle));
        glCheck(glViewport(0, 0, m_width, m_height));
    }

    void Framebuffer::bindTexture() const
    {
        m_texture.bind();
    }

    void Framebuffer::reset()
    {
        m_handle = 0;
    }

    void unbindFramebuffers(unsigned windowWidth, unsigned windowHeight)
    {
        glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        glCheck(glViewport(0, 0, windowWidth, windowHeight));
    }
} // namespace gl
