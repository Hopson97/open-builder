#include "application.h"

#include <SFML/System/Clock.hpp>
#include <common/launch_config.h>
#include <thread>

namespace server {
    Application::Application(const LaunchConfig &config, Port port)
        : m_server(config.serverOptions.maxConnections, port)
    {
    }

    void Application::run(sf::Time timeout)
    {
        sf::Clock timeoutClock;
        sf::Clock deltaClock;
        while (m_isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(25));

            m_server.recievePackets();
            update(deltaClock.restart());
            m_server.sendPackets();

            if (m_server.connectedPlayes() == 0) {
                if (timeoutClock.getElapsedTime() > timeout) {
                    m_isRunning = false;
                }
                else {
                    timeoutClock.restart();
                }
            }
        }
    }

    void Application::update(sf::Time deltaTime)
    {
        // update here
        (void)deltaTime;
        m_server.updatePlayers();
    }
} // namespace server
