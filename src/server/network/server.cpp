#include "server.h"

#include "../lua/server_lua_api.h"
#include "../world/terrain_generation.h"
#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <common/debug.h>
#include <common/util/obd_parser.h>
#include <iostream>
#include <thread>

Server::Server()
    : NetworkHost("Server")
    , m_worldSize(16)
    , m_luaCallbacks(m_script)
{
    // clang-format off
    m_commandDispatcher.addCommand(ServerCommand::VoxelEdit, &Server::onVoxelEdit);
    m_commandDispatcher.addCommand(ServerCommand::PlayerPosition, &Server::onPlayerPosition);
    m_commandDispatcher.addCommand(ServerCommand::PlayerSkin, &Server::onPlayerSkin);
    // clang-format on

    // Add all the API needed to the Lua engine
    // (Stuff that Lua calls that is defined on the C++ side)
    luaInitDataApi(m_script, m_biomeData, m_voxelData);
    luaInitWorldApi(m_script);

    // Load game in this order
    // Voxels then Biomes
    // Done this way as voxel types are a dependancy of biomes
    m_script.lua["path"] = "game/";
    m_script.runLuaFile("game/voxels.lua");
    m_script.runLuaFile("game/biomes.lua");

    m_voxelData.initCommonVoxelTypes();

    int seed = generateSeed("test");

    for (int z = 0; z < m_worldSize; z++) {
        for (int x = 0; x < m_worldSize; x++) {
            generateTerrain(m_world.chunks, x, z, m_voxelData, m_biomeData, seed,
                            m_worldSize);
        }
    }
}

void Server::sendChunk(peer_id_t peerId, const ChunkPosition& position)
{
    if (!m_connectedClients[peerId].connected) {
        return;
    }

    const Chunk& chunk = [this, &position]() {
        if (m_world.chunks.hasChunk(position)) {
            return m_world.chunks.getChunk(position);
        }
        else {
            Chunk& c = m_world.chunks.addChunk(position);
            return c;
        }
    }();

    // Create the chunk-data packet
    sf::Packet packet;
    packet << ClientCommand::ChunkData << chunk.getPosition().x << chunk.getPosition().y
           << chunk.getPosition().z;

    auto compressedChunk = compressVoxelData(chunk.voxels);
    packet << static_cast<u32>(compressedChunk.size());
    for (auto& voxel : compressedChunk) {
        packet << voxel.first << voxel.second;
    }

    // Send chunk data to client
    sendToPeer(m_connectedClients[peerId].peer, packet, 1, ENET_PACKET_FLAG_RELIABLE);
}

void Server::sendPlayerSkin(peer_id_t peerId, std::optional<peer_id_t> toPeer)
{
    sf::Packet skinPacket;
    skinPacket << ClientCommand::NewPlayerSkin;
    skinPacket << peerId;

    skinPacket.append(m_entities[peerId].m_skinData.data(), 8192);

    if (!toPeer.has_value()) {
        broadcastToPeers(skinPacket, 0, ENET_PACKET_FLAG_RELIABLE);
    }
    else {
        sendToPeer(m_connectedClients[toPeer.value()].peer, skinPacket, 0,
                   ENET_PACKET_FLAG_RELIABLE);
    }
}

void Server::sendGameData(peer_id_t peerId)
{
    sf::Packet packet;
    packet << ClientCommand::GameRegistryData;

    auto& data = m_voxelData.getVoxelData();
    packet << static_cast<u16>(data.size());
    for (auto& voxel : data) {
        u8 mesh = static_cast<u8>(voxel.meshStyle);
        u8 type = static_cast<u8>(voxel.type);
        u8 isCollidable = static_cast<u8>(voxel.isCollidable);
        packet << voxel.name;
        packet << voxel.topTexture;
        packet << voxel.sideTexture;
        packet << voxel.bottomTexture;
        packet << mesh;
        packet << type;
        packet << isCollidable;
    }

    sendToPeer(m_connectedClients[peerId].peer, packet, 0, ENET_PACKET_FLAG_RELIABLE);
}

void Server::onPeerConnect(ENetPeer* peer)
{
    int slot = findEmptySlot();
    if (slot >= 0) {
        peer_id_t id = static_cast<peer_id_t>(slot);

        // Send client back their id
        sf::Packet packet;
        packet << ClientCommand::PeerId << id;
        NetworkHost::sendToPeer(peer, packet, 0, ENET_PACKET_FLAG_RELIABLE);

        // Add peer
        addPeer(peer, id);

        // Send them the game data
        sendGameData(id);

        // Broadcast the connection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerJoin << id;
        broadcastToPeers(announcement, 0, ENET_PACKET_FLAG_RELIABLE);

        // Send the spawn chunks
        sf::Packet spawn;
        auto& player = m_entities[id];
        player.position = findPlayerSpawnPosition();
        player.m_skinData.resize(8192);
        spawn << ClientCommand::SpawnPoint << player.position.x << player.position.y
              << player.position.z;
        sendToPeer(peer, spawn, 0, ENET_PACKET_FLAG_RELIABLE);

        m_luaCallbacks.runPlayerJoinCallbacks();

        // Send chunks around the player to the client (Spawn chunks)
        // "Radius" and "player chunk" position
        int r = 2;
        ChunkPosition pc = worldToChunkPosition(player.position);
        for (int cy = pc.y - r; cy <= pc.y + r; cy++) {
            for (int cz = pc.z - r; cz < pc.z + r; cz++) {
                for (int cx = pc.x - r; cx < pc.z + r; cx++) {
                    sendChunk(id, {cx, cy, cz});
                }
            }
        }

        // Send the inital world to the client
        for (auto& chunk : m_world.chunks.chunks()) {
            sendChunk(id, chunk.second.getPosition());
        }

        // Send the peer other player's skins
        // (Note: Could this overwhelm the player's buffer?)
        for (unsigned p_id = 0; p_id < m_entities.size(); p_id++) {
            if (m_entities[p_id].hasSkin) {
                sendPlayerSkin(p_id, id);
            }
        }
    }
}

void Server::onPeerDisconnect(ENetPeer* peer)
{
    removePeer(peer->connectID);
}

void Server::onPeerTimeout(ENetPeer* peer)
{
    removePeer(peer->connectID);
}

void Server::onCommandRecieve([[maybe_unused]] ENetPeer* peer, sf::Packet& packet,
                              command_t command)
{
    m_commandDispatcher.execute(*this, command, packet);
}

void Server::onPlayerPosition(sf::Packet& packet)
{
    peer_id_t id = 0;
    packet >> id;
    packet >> m_entities[id].position.x >> m_entities[id].position.y >>
        m_entities[id].position.z;
}

void Server::onVoxelEdit(sf::Packet& packet)
{
    VoxelPosition position;
    voxel_t voxel;
    packet >> position.x >> position.y >> position.z >> voxel;
    m_world.voxelUpdates.push_back({position, voxel});
}

void Server::onPlayerSkin(sf::Packet& packet)
{
    if (packet.getDataSize() != (sizeof(command_t) + sizeof(peer_id_t) + 8192)) {
        LOG("Server", "Player Skin Packet is of an invalid size");
        return;
    }

    peer_id_t id = 0;
    packet >> id;

    sf::Uint8* skinPixels =
        (sf::Uint8*)packet.getData() + sizeof(command_t) + sizeof(peer_id_t);
    std::vector<sf::Uint8> newPixels(skinPixels, skinPixels + 8192);

    m_entities[id].m_skinData.swap(newPixels);
    m_entities[id].hasSkin = true;

    sendPlayerSkin(id);
}

void Server::update()
{
    {
        sf::Packet packet;
        if (m_world.voxelUpdates.size() > 0) {

            u16 size = static_cast<u16>(m_world.voxelUpdates.size());
            packet << ClientCommand::VoxelUpdate << size;

            for (auto& voxelUpdate : m_world.voxelUpdates) {
                auto chunkPosition = toChunkPosition(voxelUpdate.position);
                m_world.chunks.ensureNeighbours(chunkPosition);
                m_world.chunks.setVoxel(voxelUpdate.position, voxelUpdate.voxel);

                packet << voxelUpdate.position.x << voxelUpdate.position.y
                       << voxelUpdate.position.z << voxelUpdate.voxel;
            }
            // @TODO: Try find a way to not send voxel updates to players
            // that created them
            broadcastToPeers(packet, 0, ENET_PACKET_FLAG_RELIABLE);
            m_world.voxelUpdates.clear();
        }
    }

    // Player positions
    {
        sf::Packet packet;
        u16 count = NetworkHost::getConnectedPeerCount();
        packet << ClientCommand::Snapshot << count;
        for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
            if (m_connectedClients[i].connected) {
                packet << static_cast<peer_id_t>(i) << m_entities[i].position.x
                       << m_entities[i].position.y << m_entities[i].position.z;
            }
        }
        broadcastToPeers(packet, 0, 0);
    }
}

int Server::findEmptySlot() const
{
    for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
        if (!m_connectedClients[i].connected) {
            return i;
        }
    }
    return -1;
}

void Server::addPeer(ENetPeer* peer, peer_id_t id)
{
    LOGVAR("Server", "New Peer, Peer Id:", (int)id);
    m_connectedClients[id].peer = peer;
    m_connectedClients[id].connected = true;
    m_connectedClients[id].entityId = id;
}

void Server::removePeer(u32 connectionId)
{
    auto itr = std::find_if(m_connectedClients.begin(), m_connectedClients.end(),
                            [this, &connectionId](auto& conn) {
                                return conn.peer && conn.peer->connectID == connectionId;
                            });

    assert(itr != m_connectedClients.cend());
    if (itr != m_connectedClients.cend()) {
        LOGVAR("Server", "Client disconnected, Peer Id:", (int)itr->entityId);
        m_luaCallbacks.runPlayerLeaveCallbacks();

        m_entities[itr->entityId].active = false;
        m_entities[itr->entityId].hasSkin = false;
        itr->connected = false;
        itr->peer = nullptr;

        // Broadcast the disconnection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerLeave << itr->entityId;
        broadcastToPeers(announcement, 0, ENET_PACKET_FLAG_RELIABLE);

        itr->entityId = 0;
    }
}

glm::vec3 Server::findPlayerSpawnPosition()
{
    int x = (CHUNK_SIZE * m_worldSize) / 2;
    int z = (CHUNK_SIZE * m_worldSize) / 2;

    for (int chunkY = m_worldSize - 1; chunkY >= 0; chunkY--) {
        auto chunkPosition = worldToChunkPosition({x, 0, z});
        chunkPosition.y = chunkY;
        auto& spawn = m_world.chunks.getChunk(chunkPosition);

        for (int voxelY = CHUNK_SIZE - 1; voxelY >= 0; voxelY--) {
            auto voxelPosition = toLocalVoxelPosition({x, 0, z});
            voxelPosition.y = voxelY;
            if (spawn.qGetVoxel(voxelPosition) != 0) {
                auto worldY = chunkY * CHUNK_SIZE + voxelY + 3;
                return {x, worldY, z};
            }
        }
    }
    return {x, CHUNK_SIZE * m_worldSize, z};
}
