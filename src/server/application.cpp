#include "application.h"

#include <common/launch_config.h>
#include <iostream>
#include <thread>
#include "network/server.h"
#include "world/entity.h"
#include "world/chunk/chunk.h"

namespace server {
    void runServerApp(const LaunchConfig &config, port_t port, sf::Time timeout)
    {
        // Initilize the objects
        int maxConnections = config.serverOptions.maxConnections;
        EntityArray entities;
        Server server(maxConnections, port, entities);
        std::vector<Chunk> chunks;
        chunks.reserve(WORLD_SIZE * WORLD_SIZE);
        std::cout << "Server started on port " << port << "." << std::endl;

        // Initilize the world
        std::cout << "Generating world\n";
        for (unsigned i = maxConnections; i < entities.size(); i++) {
            entities[i].isAlive = true;
        }
        for (int z = 0; z < WORLD_SIZE; ++z) {
            for (int x = 0; x < WORLD_SIZE; ++x) {
                auto &c = chunks.emplace_back(ChunkPosition(x, z));
                c.generateTerrain();
            }
        }
        std::cout << "World has been created.\n" << std::endl;

        bool temp = false;

        // Start the main loop
        sf::Clock timeoutClock;
        sf::Clock deltaClock;
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // Update
            auto deltaTime = deltaClock.restart();
            server.recievePackets();
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
            auto &payload = statePacket.payload;
            payload << static_cast<u16>(entities.size());
            for (entityid_t i = 0; i < entities.size(); i++) {
                if (entities[i].isAlive) {
                    Entity &entity = entities[i];

                    payload << i;
                    payload << entity.position.x << entity.position.y
                            << entity.position.z;
                    payload << entity.rotation.x << entity.rotation.y;
                }
            }
            server.sendToAllClients(statePacket);

            //@TODO Move this and work out how to "do it correcty"
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
