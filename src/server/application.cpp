#include "application.h"

#include "network/server.h"
#include "world/chunk/chunk.h"
#include "world/entity.h"
#include "server_config.h"
#include <iostream>
#include <thread>

namespace server {
    void runServerApp(const Config &config, port_t port, sf::Time timeout)
    {
        // Initilize the objects
        int maxConnections = config.maxConnections;
        EntityArray entities;
        Server server(maxConnections, config.port, entities);
        std::vector<Chunk> chunks;
        chunks.reserve(WORLD_SIZE * WORLD_SIZE);

        // Initilize the world
        for (unsigned i = maxConnections; i < entities.size(); i++) {
            entities[i].isAlive = true;
        }
        for (int z = 0; z < WORLD_SIZE; ++z) {
            for (int x = 0; x < WORLD_SIZE; ++x) {
                auto &c = chunks.emplace_back(ChunkPosition(x, z));
                c.generateTerrain();
            }
        }

        // Start the main loop
        sf::Clock timeoutClock;
        sf::Clock deltaClock;
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // Update
            auto deltaTime = deltaClock.restart();
            server.receivePackets();
            server.resendPackets();
            server.updatePlayers();
            int id = 0;
            for (auto &entity : entities) {
                if (entity.isAlive) {
                    entity.position += entity.velocity * deltaTime.asSeconds();
                    entity.velocity.x *= 0.8f;
                    entity.velocity.z *= 0.8f;
                    if (id >= maxConnections) {
                        entity.tick();
                    }
                }
                id++;
            }

            auto statePacket = server.createPacket(CommandToClient::WorldState,
                                                   Packet::Flag::None);
            statePacket.payload << static_cast<u16>(entities.size());
            for (entityid_t i = 0; i < entities.size(); i++) {
                if (entities[i].isAlive) {
                    Entity &entity = entities[i];

                    statePacket.payload << i;
                    statePacket.payload << entity.position.x
                                        << entity.position.y
                                        << entity.position.z;
                    statePacket.payload << entity.rotation.x
                                        << entity.rotation.y;
                }
            }
            server.sendToAllClients(statePacket);

            //@TODO Move this and work out how to "do it correcty"
            static bool temp = false;
            if (server.connectedPlayes() != 0 && !temp) {
                std::cout << "Sending chunks\n";
                int i = 0;
                for (auto &chunk : chunks) {
                    chunk.sendChunks(server);
                    i++;
                }
                std::cout << "Server sent: " << i << "chunks\n";

                temp = true;
            }

            // Check for timeout
            if (server.connectedPlayes() == 0) {
                if (timeoutClock.getElapsedTime() > timeout) {
                    break;
                }
            }
            else {
                timeoutClock.restart();
            }
        }
    }
} // namespace server
