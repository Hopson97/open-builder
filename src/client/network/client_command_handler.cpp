#include "client.h"

#include "../world/world.h"

namespace client {
    void Client::handleWorldState(sf::Packet &packet)
    {
        u16 count;
        packet >> count;
        for (unsigned i = 0; i < count; i++) {
            entityid_t id;
            packet >> id;
            auto &entity = mp_world.entities[id];
            auto &transform = entity.transform;

            entity.alive = true;
            packet >> transform.position.x;
            packet >> transform.position.y;
            packet >> transform.position.z;
            float rotx, roty;
            packet >> rotx;
            packet >> roty;

            if (id != m_clientId) {
                transform.rotation.x = rotx;
                transform.rotation.y = roty;
            }
        }
    }

    void Client::handleChunkData(sf::Packet &packet)
    {
        ChunkPosition position;
        packet >> position.x >> position.y >> position.z;
        Chunk chunk(position);
        packet >> chunk;

        mp_world.chunks.try_emplace(position, std::move(chunk));
        mp_world.chunkStates.try_emplace(position,
                                         ChunkState::Flag::NeedsNewMesh);
    }

    void Client::handlePlayerJoin(sf::Packet &packet)
    {
        peer_id_t id;
        packet >> id;
        mp_world.entities[id].alive = true;

        std::cout << "Player has joined: " << (int)id << std::endl;
    }

    void Client::handlePlayerLeave(sf::Packet &packet)
    {
        peer_id_t id;
        packet >> id;
        mp_world.entities[id].alive = false;

        std::cout << "Player has left: " << (int)id << std::endl;
    }
} // namespace client