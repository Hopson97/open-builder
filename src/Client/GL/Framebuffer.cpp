#include "Framebuffer.h"

#include "GLDebug.h"
#include <cassert>
#include <stdexcept>

namespace glpp {
    Framebuffer::Framebuffer(GLuint width, GLuint height)
        : m_width(width)
        , m_height(height)
    {
        glCheck(glGenFramebuffers(1, &m_framebuffer));
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_textureAttachments = std::move(other.m_textureAttachments);
        m_renderbuffer = other.m_renderbuffer;
        m_framebuffer = other.m_framebuffer;

        other.m_width = 0;
        other.m_height = 0;
        other.m_renderbuffer = 0;
        other.m_framebuffer = 0;

        return *this;
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept
        : m_textureAttachments{std::move(other.m_textureAttachments)}
        , m_width{other.m_width}
        , m_height{other.m_height}
        , m_renderbuffer{other.m_renderbuffer}
        , m_framebuffer{other.m_framebuffer}
    {
        other.m_width = 0;
        other.m_height = 0;
        other.m_renderbuffer = 0;
        other.m_framebuffer = 0;
    }

    Framebuffer::~Framebuffer()
    {
        glCheck(glDeleteFramebuffers(1, &m_framebuffer));
        glCheck(glDeleteRenderbuffers(1, &m_renderbuffer));
    }

    void Framebuffer::attachTexture()
    {
        Texture2d& texture = m_textureAttachments.emplace_back();
        texture.bind();
        texture.create(m_width, m_height);

        glCheck(glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_textureAttachments.size() - 1,
            GL_TEXTURE_2D, texture.textureId(), 0));
    }

    void Framebuffer::finalise()
    {
        glCheck(glGenRenderbuffers(1, &m_renderbuffer));
        glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer));
        glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width,
                                      m_height));

        glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                          GL_RENDERBUFFER, m_renderbuffer));

        std::vector<GLuint> attachments;
        for (unsigned i = 0; i < m_textureAttachments.size(); i++) {
            attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
        }
        glDrawBuffers(attachments.size(), attachments.data());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Renderbuffer failed to be created");
        }
    }

    void Framebuffer::bind() const
    {
        assert(m_width != 0);
        assert(m_height != 0);
        assert(m_framebuffer != 0);

        glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));
        glCheck(glViewport(0, 0, m_width, m_height));
    }

    void Framebuffer::bindTexture(unsigned index) const
    {
        assert(index < m_textureAttachments.size());
        m_textureAttachments[index].bind();
    }

    void Framebuffer::unbind(GLuint width, GLuint height)
    {
        glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        glCheck(glViewport(0, 0, width, height));
    }

} // namespace glpp