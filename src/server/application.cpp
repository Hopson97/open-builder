#include "application.h"

#include <SFML/System/Clock.hpp>
#include <common/launch_config.h>
#include <common/network/commands.h>
#include <iostream>
#include <thread>

namespace server {
    Application::Application(const LaunchConfig &config, Port port)
        : m_server(config.serverOptions.maxConnections, port, m_entities)
    {
        std::cout << "Server started on port " << port << "." << std::endl;

        m_entities[m_server.maxConnections() + 1].isAlive = true;
    }

    void Application::run(sf::Time timeout)
    {
        sf::Clock timeoutClock;
        sf::Clock deltaClock;
        while (m_isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(25));

            m_server.recievePackets();
            update(deltaClock.restart());
            sendState();

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
        m_server.updatePlayers();

        for (auto &entity : m_entities) {
            if (entity.isAlive) {
                entity.position += entity.velocity * deltaTime.asSeconds();
                entity.velocity.x *= 0.95f;
                entity.velocity.z *= 0.95f;
            }
        }
    }

    void Application::sendState()
    {
        auto statePacket = createCommandPacket(CommandToClient::WorldState);
        statePacket << static_cast<u16>(m_entities.size());
        for (u16 i = 0; i < m_entities.size(); i++) {
            if (m_entities[i].isAlive) {
                Entity &entity = m_entities[i];

                statePacket << i;
                statePacket << entity.position.x << entity.position.y
                            << entity.position.z;
                statePacket << entity.rotation.x << entity.rotation.y;
            }
        }
        m_server.sendToAllClients(statePacket);
    }
} // namespace server
