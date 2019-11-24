#include "fps_counter.h"

namespace client {
    void FPSCounter::update()
    {
        m_frameCount++;
    }

    float FPSCounter::currentFps() const
    {
        return m_frameCount / m_timer.getElapsedTime().asSeconds();
    }

    float FPSCounter::currentMsPerFrame() const
    {
        return static_cast<float>(m_timer.getElapsedTime().asMilliseconds()) /
               m_frameCount;
    }

    void FPSCounter::reset()
    {
        m_frameCount = 0;
        m_timer.restart();
    }
} // namespace client