#include "../client.h"

#include <common/debug.h>
#include <common/world/chunk.h>
#include <thread>

void Client::sendPlayerPosition(const glm::vec3 &position)
{
    sf::Packet packet;
    packet << ServerCommand::PlayerPosition << NetworkHost::getPeerId()
           << position.x << position.y << position.z;
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0, 0);
}

void Client::sendBlockUpdate(const BlockUpdate &update)
{
    sf::Packet packet;
    packet << ServerCommand::BlockEdit << update.position.x << update.position.y
           << update.position.z << update.block;
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0, 0);
}

void Client::sendPlayerSkin(const sf::Image &playerSkin)
{
    // Check the image is the right size
    if (playerSkin.getSize() != sf::Vector2u(32, 64)) {
        LOG("Client", "Player's skin has the wrong dimensions");
        return;
    }

    sf::Packet packet;
    packet << ServerCommand::PlayerSkin << NetworkHost::getPeerId();
    packet.append(playerSkin.getPixelsPtr(), 8192);
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0,
                            ENET_PACKET_FLAG_RELIABLE);
}

void Client::onPeerConnect([[maybe_unused]] ENetPeer *peer)
{
}

void Client::onPeerDisconnect([[maybe_unused]] ENetPeer *peer)
{
    m_status = EngineStatus::ExitServerDisconnect;
}

void Client::onPeerTimeout([[maybe_unused]] ENetPeer *peer)
{
    m_status = EngineStatus::ExitServerTimeout;
}

void Client::onCommandRecieve([[maybe_unused]] ENetPeer *peer,
                              sf::Packet &packet, command_t command)
{
    m_commandDispatcher.execute(*this, command, packet);
}

void Client::onPlayerJoin(sf::Packet &packet)
{
    peer_id_t id = 0;
    packet >> id;
    m_entities[id].active = true;

    LOGVAR("Client", "Player joined, client id: ", (int)id);
}

void Client::onPlayerLeave(sf::Packet &packet)
{
    peer_id_t id = 0;
    packet >> id;
    m_entities[id].active = false;

    LOGVAR("Client", "Player left, client id: ", (int)id);
}

void Client::onSnapshot(sf::Packet &packet)
{
    u16 updateEntityCount = 0;
    packet >> updateEntityCount;
    for (u16 i = 0; i < updateEntityCount; i++) {
        peer_id_t id = 0;
        float x, y, z;
        packet >> id >> x >> y >> z;
        if (id != NetworkHost::getPeerId()) {
            auto *p = &m_entities[id];
            p->position = {x, y, z};
            p->active = true;
        }
    }
}

void Client::onChunkData(sf::Packet &packet)
{
    // Get position of the recieved chunk, and create a chunk
    ChunkPosition position;
    packet >> position.x >> position.y >> position.z;

    if (!m_chunks.manager.hasChunk(position)) {

        Chunk &chunk = m_chunks.manager.addChunk(position);

        u32 size;
        CompressedBlocks compressed;
        packet >> size;
        for (u32 i = 0; i < size; i++) {
            block_t type;
            u16 count;
            packet >> type >> count;
            compressed.emplace_back(type, count);
        }

        // Uncompress the block data
        chunk.blocks = decompressBlockData(compressed);

        // Add to chunk updates
        m_chunks.updates.push_back(position);
    }
}

void Client::onSpawnPoint(sf::Packet &packet)
{
    assert(mp_player);
    if (mp_player) {
        packet >> mp_player->position.x >> mp_player->position.y >>
            mp_player->position.z;
    }
}

void Client::onBlockUpdate(sf::Packet &packet)
{
    u16 count = 0;
    packet >> count;
    for (u32 i = 0; i < count; i++) {
        BlockUpdate blockUpdate;
        packet >> blockUpdate.position.x >> blockUpdate.position.y >>
            blockUpdate.position.z >> blockUpdate.block;
        m_chunks.blockUpdates.push_back(blockUpdate);
    }
}

void Client::onPlayerSkinReceive(sf::Packet &packet)
{
    peer_id_t id = 0;
    packet >> id;

    LOGVAR("Client", "Received skin for peer", (int)id);

    sf::Uint8 *skinPixels =
        (sf::Uint8 *)packet.getData() + sizeof(command_t) + sizeof(peer_id_t);
    m_entities[id].playerSkin.create(32, 64, skinPixels);
}

void Client::onGameRegistryData(sf::Packet &packet)
{
    //  ====
    //  Get all blocks from the server
    //
    // Maps tewxture names to their respective IDs in the
    // OpenGL texture array
    std::unordered_map<std::string, GLuint> textureMap;
    auto getTexture = [&textureMap, this](const std::string &name) {
        auto itr = textureMap.find(name);
        if (itr == textureMap.end()) {
            auto id = m_voxelTextures.addTexture(name);
            textureMap.emplace(name, id);
            return id;
        }
        return itr->second;
    };

    u16 numBlocks;
    packet >> numBlocks;
    // todo
    // 1. Need to somehow work out the exact amount of textures needed
    // 2. Need to pass in the actual texture pack resolution
    m_voxelTextures.create(numBlocks * 3, 16);

    const std::string texturePath =
        "texture_packs/" + m_texturePack + "/blocks/";
    for (u16 i = 0; i < numBlocks; i++) {
        std::string name;
        std::string textureTop;
        std::string textureSide;
        std::string textureBottom;

        u8 meshStyle = 0;
        u8 type = 0;
        u8 isCollidable = 0;

        packet >> name;
        packet >> textureTop;
        packet >> textureSide;
        packet >> textureBottom;
        packet >> meshStyle;
        packet >> type;
        packet >> isCollidable;

        VoxelData voxelData;
        voxelData.name = name;
        voxelData.topTextureId = getTexture(texturePath + textureTop);
        voxelData.sideTextureId = getTexture(texturePath + textureSide);
        voxelData.bottomTextureId = getTexture(texturePath + textureBottom);

        voxelData.meshStyle = static_cast<VoxelMeshStyle>(meshStyle);
        voxelData.type = static_cast<VoxelType>(type);
        voxelData.isCollidable = isCollidable;

        m_voxelData.addVoxelData(voxelData);
    }

    m_hasReceivedGameData = true;
}