#pragma once

#include <SFML/System/Clock.hpp>

namespace client {
    /**
     * @brief Utility class for performance benchmarking
     * eg Frames per second, MS per frame
     *
     */
    class FPSCounter final {
      public:
        /// Update the frame counter, must be called once per frame
        void update();

        float currentFps() const;
        float currentMsPerFrame() const;

        void reset();

      private:
        sf::Clock m_timer;
        float m_frameCount = 0;
    };
} // namespace client
